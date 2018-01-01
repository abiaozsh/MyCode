import tensorflow as tf
import numpy as np
from six.moves import xrange
#import numpy
#from PIL import Image


def openEmptyFileR(filename):
    try:
        return open(filename, 'r')
    except:
        return None


def openEmptyFileW(filename):
    try:
        return open(filename, 'w')
    except:
        return None
#  hidden = tf.nn.dropout(hidden, 0.5, seed=SEED)

def onehot(count,values):
    return np.eye(count)[values]

def Conv2FC_Reshape(conv):
    _shape = conv.get_shape().as_list()
    reshape = tf.reshape(conv, [_shape[0], _shape[1] * _shape[2] * _shape[3]])
    return reshape


def FC2Conv_Reshape(fc, h, w, c):
    _shape = fc.get_shape().as_list()
    reshape = tf.reshape(fc, [_shape[0], h, w, c])
    return reshape

def getParam(_params):
    _list = []
    for item in _params:
        _list.append(item.biases)
        _list.append(item.weights)
    return _list

def addlist(_list,item):
    _list.append(item)
    return item

class DeConv:
    #biases
    #weights
    #inDepth,outDepth,filterSize
    
    def __init__(self,inDepth,outDepth,filterSize,loadFromFile = None):
        self.inDepth = inDepth
        self.outDepth = outDepth
        self.filterSize = filterSize
        self.loadFromFile = loadFromFile
        if loadFromFile:
            biasarray = np.ndarray([self.outDepth], np.float32)
            
            for i in xrange(0, self.outDepth):
                line = loadFromFile.readline()
                biasarray[i] = float(line)
            
            self.biasesROM = biasarray
    
            warray = np.ndarray([self.filterSize, self.filterSize, self.outDepth, self.inDepth], np.float32)
            
            for j in xrange(0, self.outDepth):
                for x in xrange(0, self.filterSize):#H
                    for y in xrange(0, self.filterSize):#W
                        for i in xrange(0, self.inDepth):
                            line = loadFromFile.readline()
                            warray[x, y, j, i] = float(line)
            
            self.weightsROM = warray
        else:
            self.biasesROM = tf.constant(0.1, shape=[outDepth], dtype=tf.float32)
            dev = 1/(filterSize * filterSize * outDepth)**0.5
            self.weightsROM = tf.truncated_normal([filterSize, filterSize, outDepth, inDepth], stddev=dev, dtype=tf.float32)

        self.biases = tf.Variable(self.biasesROM)
        self.weights = tf.Variable(self.weightsROM)

    def save(self,sess,saveToFile):
        biasarray, warray = self.save_getParam(sess)
        self.save_ToFile(biasarray, warray, saveToFile)

    def getSaver(self,sess):
        biasarray, warray = self.save_getParam(sess)
        
        def saver(saveToFile):
            self.save_ToFile(biasarray, warray, saveToFile)
            
        return saver
    
    def save_getParam(self,sess):
        biasarray = sess.run(self.biases)
        warray = sess.run(self.weights)
        return biasarray, warray
    
    def save_ToFile(self, biasarray, warray, saveToFile):
        for i in xrange(0, self.outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n")
        saveToFile.flush()
        for j in xrange(0, self.outDepth):
            for x in xrange(0, self.filterSize):
                for y in xrange(0, self.filterSize):
                    for i in xrange(0, self.inDepth):
                        val = warray[x, y, j, i]
                        saveToFile.write(str(val) + "\n")
        saveToFile.flush()
        
    def getLayer(self, inputT, height, width, convStride ,isRelu=True, fixed = False):
        
        if fixed:
            if self.loadFromFile:
                b = tf.constant(self.biasesROM)
                w = tf.constant(self.weightsROM)
            else:
                b = self.biasesROM
                w = self.weightsROM
        else:
            b = self.biases
            w = self.weights
            
        _out = tf.nn.conv2d_transpose(inputT, w, output_shape=[int(inputT.shape[0]),height,width,self.outDepth], strides=[1, convStride, convStride, 1], padding="SAME")

        _out = tf.nn.bias_add(_out, b)
        
        if isRelu:
            _out = tf.nn.leaky_relu(_out)

        return _out

class Conv:
    
    def __init__(self,inDepth,outDepth,filterSize,biasInitVal = 0.1,loadFromFile = None):
        self.inDepth = inDepth
        self.outDepth = outDepth
        self.filterSize = filterSize
        self.loadFromFile = loadFromFile
        
        if loadFromFile:
            biasarray = np.ndarray([outDepth], np.float32)
    
            for i in xrange(0, outDepth):
                line = loadFromFile.readline()
                biasarray[i] = float(line)
    
            self.biasesROM = biasarray
    
            warray = np.ndarray([filterSize, filterSize, inDepth, outDepth], np.float32)
          
            for j in xrange(0, self.outDepth):
                for x in xrange(0, self.filterSize):
                    for y in xrange(0, self.filterSize):
                        for i in xrange(0, self.inDepth):
                            line = loadFromFile.readline()
                            warray[x, y, i, j] = float(line)
    
            self.weightsROM = warray
            
        else:
            self.biasesROM = tf.constant(biasInitVal, shape=[outDepth], dtype=tf.float32)
            dev = 1/(filterSize * filterSize * outDepth)**0.5
            self.weightsROM = tf.truncated_normal([filterSize, filterSize, inDepth, outDepth], stddev=dev, dtype=tf.float32)

        self.biases = tf.Variable(self.biasesROM)
        self.weights = tf.Variable(self.weightsROM)

    def save(self,sess,saveToFile):
        biasarray, warray = self.save_getParam(sess)
        self.save_ToFile(biasarray, warray, saveToFile)

    def getSaver(self,sess):
        biasarray, warray = self.save_getParam(sess)
        
        def saver(saveToFile):
            self.save_ToFile(biasarray, warray, saveToFile)
            
        return saver

    def save_getParam(self,sess):
        biasarray = sess.run(self.biases)
        warray = sess.run(self.weights)
        return biasarray, warray
    
    def save_ToFile(self, biasarray, warray, saveToFile):
        for i in xrange(0, self.outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n")
        saveToFile.flush()
        for j in xrange(0, self.outDepth):
            for x in xrange(0, self.filterSize):
                for y in xrange(0, self.filterSize):
                    for i in xrange(0, self.inDepth):
                        val = warray[x, y, i, j]
                        saveToFile.write(str(val) + "\n")
        saveToFile.flush()

    def getLayer(self, inputT, convStride, poolSize,isRelu=True, fixed = False):
        
        if fixed:
            if self.loadFromFile:
                b = tf.constant(self.biasesROM)
                w = tf.constant(self.weightsROM)
            else:
                b = self.biasesROM
                w = self.weightsROM
        else:
            b = self.biases
            w = self.weights

        _out = tf.nn.conv2d(inputT, w, strides=[1, convStride, convStride, 1], padding="SAME")
    
        _out = tf.nn.bias_add(_out, b)
        
        if isRelu:
            _out = tf.nn.leaky_relu(_out)

        if poolSize > 1:
            _out = tf.nn.max_pool(_out, ksize=[1, poolSize, poolSize, 1], strides=[1, poolSize, poolSize, 1], padding="SAME")
        return _out

class FC:
    def __init__(self,inDepth,outDepth,loadFromFile = None):
        self.inDepth = inDepth
        self.outDepth = outDepth
        self.loadFromFile = loadFromFile

        if loadFromFile:
            biasarray = np.ndarray([outDepth], np.float32)
    
            for i in xrange(0, outDepth):
                line = loadFromFile.readline()
                biasarray[i] = float(line)
    
            self.biasesROM = biasarray
    
            warray = np.ndarray([inDepth, outDepth], np.float32)
    
            for j in xrange(0, outDepth):
                for i in xrange(0, inDepth):
                    line = loadFromFile.readline()
                    warray[i, j] = float(line)
    
            self.weightsROM = warray
            
        else:
            self.biasesROM = tf.constant(0.1, shape=[outDepth], dtype=tf.float32)
            dev = 1/(outDepth)**0.5
            self.weightsROM = tf.truncated_normal([inDepth, outDepth], stddev=dev, dtype=tf.float32)
    
        self.biases = tf.Variable(self.biasesROM)
        self.weights = tf.Variable(self.weightsROM)

    def save(self,sess,saveToFile):
        biasarray, warray = self.save_getParam(sess)
        self.save_ToFile(biasarray, warray, saveToFile)
        
    def getSaver(self,sess):
        biasarray, warray = self.save_getParam(sess)
        
        def saver(saveToFile):
            self.save_ToFile(biasarray, warray, saveToFile)
            
        return saver

    def save_getParam(self,sess):
        biasarray = sess.run(self.biases)
        warray = sess.run(self.weights)
        return biasarray, warray
    
    def save_ToFile(self, biasarray, warray, saveToFile):
        for i in xrange(0, self.outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n")
        saveToFile.flush()
        for j in xrange(0, self.outDepth):
            for i in xrange(0, self.inDepth):
                val = warray[i, j]
                saveToFile.write(str(val) + "\n")
        saveToFile.flush()

    def getLayer(self, inputT, isRelu=True, fixed = False):
        
        if fixed:
            if self.loadFromFile:
                b = tf.constant(self.biasesROM)
                w = tf.constant(self.weightsROM)
            else:
                b = self.biasesROM
                w = self.weightsROM
        else:
            b = self.biases
            w = self.weights
       
        _out = tf.nn.bias_add(tf.matmul(inputT, w), b)
        
        if isRelu:
            _out = tf.nn.leaky_relu(_out)
        return _out

def npSave1d(nparray,saveToFile):
    for i in xrange(0, nparray.shape[0]):
        val = nparray[i]
        saveToFile.write(str(val) + "\n")

def npLoad1d(nparray,loadFromFile):
    for i in xrange(0, nparray.shape[0]):
        line = loadFromFile.readline()
        nparray[i] = float(line)


def npSave2d(nparray,saveToFile):
    for i in xrange(0, nparray.shape[0]):
        for j in xrange(0, nparray.shape[1]):
            val = nparray[i,j]
            saveToFile.write(str(val) + "\n")

def npLoad2d(nparray,loadFromFile):
    for i in xrange(0, nparray.shape[0]):
        for j in xrange(0, nparray.shape[1]):
            line = loadFromFile.readline()
            nparray[i,j] = float(line)

import imageio
#import scipy.misc

def saveImages(images, size, path):
    img = images
    h, w = img.shape[1], img.shape[2]
    merge_img = np.zeros((h * size[0], w * size[1], 3),dtype="float32")
    for idx, image in enumerate(images):
        i = idx % size[1]
        j = idx // size[1]
        merge_img[j*h:j*h+h, i*w:i*w+w, :] = image
    
    merge_img = np.clip(merge_img, -0.5, 0.5)
    imageio.imwrite(path, merge_img)
    return

def saveImagesMono(images, size, path):
    img = images
    h, w = img.shape[1], img.shape[2]
    merge_img = np.zeros((h * size[0], w * size[1], 1),dtype="float32")
    for idx, image in enumerate(images):
        i = idx % size[1]
        j = idx // size[1]
        merge_img[j*h:j*h+h, i*w:i*w+w, :] = image
    
    #Max value == min value, ambiguous given dtype 因为值全是0
    merge_img = np.clip(merge_img, -0.5, 0.5)
    imageio.imwrite(path, merge_img)
    return

# def saveImg(data, filename):
#     img = Image.frombytes("RGB", (data.shape[1], data.shape[0]), data)
#     img.save(filename)  # str(idx)+"-"+str(lbl)+"a.bmp"
#     
#     
# def newImage(w, h):
#     return numpy.ndarray([h, w, 3], numpy.byte)
# 
# 
# def setpixel(imgdata, x, y, r, g, b):
#     if r < 0: r = 0
#     if r > 255: r = 255
#     if g < 0: g = 0
#     if g > 255: g = 255
#     if b < 0: b = 0
#     if b > 255: b = 255
#     
#     imgdata[y, x, 0] = r
#     imgdata[y, x, 1] = g
#     imgdata[y, x, 2] = b
# 
#     
# def clearImg(imgdata):
#     for i in xrange(0, imgdata.shape[1]):
#         for j in xrange(0, imgdata.shape[2]):
#             setpixel(imgdata, i, j, 0, 0, 0)
    
def upsample(net, stride, mode='ZEROS'):
    """
    Imitate reverse operation of Max-Pooling by either placing original max values
    into a fixed postion of upsampled cell:
    [0.9] =>[[.9, 0],   (stride=2)
             [ 0, 0]]
    or copying the value into each cell:
    [0.9] =>[[.9, .9],  (stride=2)
             [ .9, .9]]
    :param net: 4D input tensor with [batch_size, width, heights, channels] axis
    :param stride:
    :param mode: string 'ZEROS' or 'COPY' indicating which value to use for undefined cells
    :return:  4D tensor of size [batch_size, width*stride, heights*stride, channels]
    """
    assert mode in ['COPY', 'ZEROS']
    with tf.name_scope('Upsampling'):
        net = _upsample_along_axis(net, 2, stride, mode=mode)
        net = _upsample_along_axis(net, 1, stride, mode=mode)
        return net


def _upsample_along_axis(volume, axis, stride, mode='ZEROS'):
    shape = volume.get_shape().as_list()
    
    assert mode in ['COPY', 'ZEROS']
    assert 0 <= axis < len(shape)
    
    target_shape = shape[:]
    target_shape[axis] *= stride
    
    padding = tf.zeros(shape, dtype=volume.dtype) if mode == 'ZEROS' else volume
    parts = [volume] + [padding for _ in range(stride - 1)]
    volume = tf.concat(parts, min(axis+1, len(shape)-1))
    
    volume = tf.reshape(volume, target_shape)
    return volume


def outprint(v):
    ret = ""
    ret += "["
    if v.shape.__len__() == 4:
        for y in xrange(0, v.shape[1]):
            ret += "["
            for x in xrange(0, v.shape[2]):
                ret += "["
                for i in xrange(0, v.shape[3]):
                    ret += str(v[0, y, x, i]) + ","
    
                ret += "]\n"
            ret += "]\n"
        ret += "]\n"
    else:
        ret += "["
        ret += "["
        for i in xrange(0, v.shape[1]):
            ret += str(v[0, i]) + ","
    
        ret += "]\n"
        ret += "]\n"
        ret += "]\n"

    print(ret)
