import tensorflow as tf
import numpy as np
from six.moves import xrange
import numpy

from PIL import Image


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


def Conv2FC_Reshape(conv):
    _shape = conv.get_shape().as_list()
    reshape = tf.reshape(conv, [_shape[0], _shape[1] * _shape[2] * _shape[3]])
    return reshape


def FC2Conv_Reshape(fc, h, w, c):
    _shape = fc.get_shape().as_list()
    reshape = tf.reshape(fc, [_shape[0], h, w, c])
    return reshape

def getParam(_params):
    list = []
    for item in _params:
        list.append(item.biases)
        list.append(item.weights)
    return list

# tf.nn.conv2d_transpose(value, filter, output_shape, strides, padding='SAME', name=None)

class DeConv:
    #biases
    #weights
    #inDepth,outDepth,filterSize
    
    def __init__(self,inDepth,outDepth,filterSize,loadFromFile = None):
        self.inDepth = inDepth
        self.outDepth = outDepth
        self.filterSize = filterSize

        if loadFromFile:
            biasarray = np.ndarray([self.outDepth], np.float32)
            
            for i in xrange(0, self.outDepth):
                line = loadFromFile.readline();
                biasarray[i] = float(line)
            
            self.biasesROM = tf.constant(biasarray)
    
            warray = np.ndarray([self.filterSize, self.filterSize, self.outDepth, self.inDepth], np.float32)
            
            for j in xrange(0, self.outDepth):
                for x in xrange(0, self.filterSize):#H
                    for y in xrange(0, self.filterSize):#W
                        for i in xrange(0, self.inDepth):
                            line = loadFromFile.readline();
                            warray[x, y, j, i] = float(line)
            
            self.weightsROM = tf.constant(warray)
        else:
            self.biasesROM = tf.constant(0.1, shape=[outDepth], dtype=tf.float32)
            self.weightsROM = tf.truncated_normal([filterSize, filterSize, outDepth, inDepth], stddev=0.1, dtype=tf.float32)

        self.biases = tf.Variable(self.biasesROM)
        self.weights = tf.Variable(self.weightsROM)

    def save(self,sess,saveToFile):
        biasarray, warray = self.save_getParam(sess)
        self.save_ToFile(biasarray, warray, saveToFile)

    def save_getParam(self,sess):
        biasarray = sess.run(self.biases)
        warray = sess.run(self.weights)
        return biasarray, warray
    
    def save_ToFile(self, biasarray, warray, saveToFile):
        for i in xrange(0, self.outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n");
        for j in xrange(0, self.outDepth):
            for x in xrange(0, self.filterSize):
                for y in xrange(0, self.filterSize):
                    for i in xrange(0, self.inDepth):
                        val = warray[x, y, j, i]
                        saveToFile.write(str(val) + "\n");

    def getLayer(self, inputT, height, width, convStride ,isRelu=True, fixed = False):
        
        if fixed:
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
    
    def __init__(self,inDepth,outDepth,filterSize,loadFromFile = None):
        self.inDepth = inDepth
        self.outDepth = outDepth
        self.filterSize = filterSize
        if loadFromFile:
            biasarray = np.ndarray([outDepth], np.float32)
    
            for i in xrange(0, outDepth):
                line = loadFromFile.readline();
                biasarray[i] = float(line)
    
            self.biasesROM = tf.constant(biasarray)
    
            warray = np.ndarray([filterSize, filterSize, inDepth, outDepth], np.float32)
          
            for j in xrange(0, self.outDepth):
                for x in xrange(0, self.filterSize):
                    for y in xrange(0, self.filterSize):
                        for i in xrange(0, self.inDepth):
                            line = loadFromFile.readline();
                            warray[x, y, i, j] = float(line)
    
            self.weightsROM = tf.constant(warray)
            
        else:
            self.biasesROM = tf.constant(0.1, shape=[outDepth], dtype=tf.float32)
            self.weightsROM = tf.truncated_normal([filterSize, filterSize, inDepth, outDepth], stddev=0.1, dtype=tf.float32)

        self.biases = tf.Variable(self.biasesROM)
        self.weights = tf.Variable(self.weightsROM)

    def save(self,sess,saveToFile):
        biasarray, warray = self.save_getParam(sess)
        self.save_ToFile(biasarray, warray, saveToFile)

    def save_getParam(self,sess):
        biasarray = sess.run(self.biases)
        warray = sess.run(self.weights)
        return biasarray, warray
    
    def save_ToFile(self, biasarray, warray, saveToFile):
        for i in xrange(0, self.outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n");
        for j in xrange(0, self.outDepth):
            for x in xrange(0, self.filterSize):
                for y in xrange(0, self.filterSize):
                    for i in xrange(0, self.inDepth):
                        val = warray[x, y, i, j]
                        saveToFile.write(str(val) + "\n");

    def getLayer(self, inputT, convStride, poolSize,isRelu=True, fixed = False):
        
        if fixed:
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

        if loadFromFile:
            biasarray = np.ndarray([outDepth], np.float32)
    
            for i in xrange(0, outDepth):
                line = loadFromFile.readline();
                biasarray[i] = float(line)
    
            self.biasesROM = tf.constant(biasarray)
    
            warray = np.ndarray([inDepth, outDepth], np.float32)
    
            for j in xrange(0, outDepth):
                for i in xrange(0, inDepth):
                    line = loadFromFile.readline();
                    warray[i, j] = float(line)
    
            self.weightsROM = tf.constant(warray)
            
        else:
            self.biasesROM = tf.constant(0.1, shape=[outDepth], dtype=tf.float32)
            self.weightsROM = tf.truncated_normal([inDepth, outDepth], stddev=0.1, dtype=tf.float32)
    
        self.biases = tf.Variable(self.biasesROM)
        self.weights = tf.Variable(self.weightsROM)

    def save(self,sess,saveToFile):
        biasarray, warray = self.save_getParam(sess)
        self.save_ToFile(biasarray, warray, saveToFile)

    def save_getParam(self,sess):
        biasarray = sess.run(self.biases)
        warray = sess.run(self.weights)
        return biasarray, warray
    
    def save_ToFile(self, biasarray, warray, saveToFile):
        for i in xrange(0, self.outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n");
        for j in xrange(0, self.outDepth):
            for i in xrange(0, self.inDepth):
                val = warray[i, j]
                saveToFile.write(str(val) + "\n");

    def getLayer(self, inputT, isRelu=True, fixed = False):
        
        if fixed:
            b = self.biasesROM
            w = self.weightsROM
        else:
            b = self.biases
            w = self.weights
       
        _out = tf.nn.bias_add(tf.matmul(inputT, w), b)
        
        if isRelu:
            _out = tf.nn.leaky_relu(_out)
        return _out



import imageio
import scipy.misc

def saveImages(images, size, path):
    img = images
    h, w = img.shape[1], img.shape[2]
    merge_img = np.zeros((h * size[0], w * size[1], 3))
    for idx, image in enumerate(images):
        i = idx % size[1]
        j = idx // size[1]
        merge_img[j*h:j*h+h, i*w:i*w+w, :] = image
    
    merge_img = np.clip(merge_img, -0.5, 0.5)
    #img = Image.frombytes("RGB", (merge_img.shape[0], merge_img.shape[1]), (merge_img + 0.5) * 255)
    #img.save("_"+path)
    #scipy.misc.imsave(path, merge_img)
    imageio.imwrite(path, merge_img)
    #imageio.imwrite()
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
    

def outprint(v):
    ret = ""
    ret += "["
    if v.shape.__len__() == 4:
        for y in xrange(0, v.shape[1]):
            ret += "[";
            for x in xrange(0, v.shape[2]):
                ret += "[";
                for i in xrange(0, v.shape[3]):
                    ret += str(v[0, y, x, i]) + ",";
    
                ret += "]\n"
            ret += "]\n"
        ret += "]\n"
    else:
        ret += "[";
        ret += "[";
        for i in xrange(0, v.shape[1]):
            ret += str(v[0, i]) + ",";
    
        ret += "]\n"
        ret += "]\n"
        ret += "]\n"

    print(ret)
