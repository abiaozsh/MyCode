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

# tf.nn.conv2d_transpose(value, filter, output_shape, strides, padding='SAME', name=None)


# output_shape [batch, height, width, in_channels]
def DeConvLayer(inputT, filterSize, output_shape, convStride, loadFromFile=None, noChange=False, isRelu=True, padding=True):
    
    inDepth = int(inputT.shape[3])
    outDepth = output_shape[3]
    
    if loadFromFile:
        biasarray = np.ndarray([outDepth], np.float32)

        for i in xrange(0, outDepth):
            line = loadFromFile.readline();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)

        warray = np.ndarray([filterSize, filterSize, outDepth, inDepth], np.float32)
      
        for i in xrange(0, inDepth):
            for x in xrange(0, filterSize):
                for y in xrange(0, filterSize):
                    for j in xrange(0, outDepth):
                        line = loadFromFile.readline();
                        warray[x, y, j, i] = float(line)

        weights = tf.constant(warray)
        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outDepth], dtype=tf.float32))
        weights = tf.truncated_normal([filterSize, filterSize, outDepth, inDepth], stddev=0.1, dtype=tf.float32)

    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
    
    if padding:
        padding = "SAME"
    else:
        padding = "VALID"
        
    #tf.nn.convolution
    
    _conv = tf.nn.conv2d_transpose(inputT, weights, output_shape=output_shape, strides=[1, convStride, convStride, 1], padding=padding)

    if isRelu:
        _relu = tf.nn.leaky_relu(tf.nn.bias_add(_conv, biases))
    else:
        _relu = tf.nn.bias_add(_conv, biases)

    _out = _relu
        
    def saver(sess, saveToFile):
        biasarray = sess.run(biases)

        for i in xrange(0, outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n");

        warray = sess.run(weights)

        for i in xrange(0, inDepth):
            for x in xrange(0, filterSize):
                for y in xrange(0, filterSize):
                    for j in xrange(0, outDepth):
                        val = warray[x, y, j, i]
                        saveToFile.write(str(val) + "\n");

    return _out, saver


def ConvLayer(inputT, filterSize, outDepth, convStride, poolSize, loadFromFile=None, noChange=False, isRelu=True, padding=True):
    
    inDepth = int(inputT.shape[3])
    
    if loadFromFile:
        biasarray = np.ndarray([outDepth], np.float32)

        for i in xrange(0, outDepth):
            line = loadFromFile.readline();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)

        warray = np.ndarray([filterSize, filterSize, inDepth, outDepth], np.float32)
      
        for j in xrange(0, outDepth):
            for x in xrange(0, filterSize):
                for y in xrange(0, filterSize):
                    for i in xrange(0, inDepth):
                        line = loadFromFile.readline();
                        warray[x, y, i, j] = float(line)

        weights = tf.constant(warray)
        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outDepth], dtype=tf.float32))
        weights = tf.truncated_normal([filterSize, filterSize, inDepth, outDepth], stddev=0.1, dtype=tf.float32)

    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
    
    if padding:
        padding = "SAME"
    else:
        padding = "VALID"
    
    _conv = tf.nn.conv2d(inputT, weights, strides=[1, convStride, convStride, 1], padding=padding)

    if isRelu:
        _relu = tf.nn.leaky_relu(tf.nn.bias_add(_conv, biases))
    else:
        _relu = tf.nn.bias_add(_conv, biases)

    if poolSize > 1:
        _out = tf.nn.max_pool(_relu, ksize=[1, poolSize, poolSize, 1], strides=[1, poolSize, poolSize, 1], padding=padding)
    else:
        _out = _relu
        
    def saver(sess, saveToFile):
        biasarray = sess.run(biases)

        for i in xrange(0, outDepth):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n");

        warray = sess.run(weights)

        for j in xrange(0, outDepth):
            for x in xrange(0, filterSize):
                for y in xrange(0, filterSize):
                    for i in xrange(0, inDepth):
                        val = warray[x, y, i, j]
                        saveToFile.write(str(val) + "\n");

    return _out, saver


def FCLayer(inputT, outCnt, isRelu=True, loadFromFile=None, noChange=False):  # inCnt,
    inCnt = int(inputT.shape[1])
    _input = inCnt
    _output = outCnt

    if loadFromFile:
        biasarray = np.ndarray([outCnt], np.float32)

        for i in xrange(0, outCnt):
            line = loadFromFile.readline();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)

        warray = np.ndarray([inCnt, outCnt], np.float32)

        for j in xrange(0, outCnt):
            for i in xrange(0, inCnt):
                line = loadFromFile.readline();
                warray[i, j] = float(line)

        weights = tf.constant(warray)
        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outCnt], dtype=tf.float32))
        weights = tf.truncated_normal([inCnt, outCnt], stddev=0.1, dtype=tf.float32)

    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
   
    layer = tf.nn.bias_add(tf.matmul(inputT, weights), biases)
    
    def saver(sess, saveToFile):

        biasarray = sess.run(biases)

        for i in xrange(0, outCnt):
            val = biasarray[i]
            saveToFile.write(str(val) + "\n");

        warray = sess.run(weights)

        for j in xrange(0, outCnt):
            for i in xrange(0, inCnt):
                val = warray[i, j]
                saveToFile.write(str(val) + "\n");

        return
    
    if isRelu:
        return tf.nn.leaky_relu(layer), saver
    else:
        return layer, saver


def saveImg(data, filename):
    img = Image.frombytes("RGBA", (data.shape[0], data.shape[1]), data)
    img.save(filename)  # str(idx)+"-"+str(lbl)+"a.bmp"
    
    
def newImage(w, h):
    return numpy.ndarray([h, w, 4], numpy.byte)


def setpixel(imgdata, x, y, r, g, b):
    if r < 0: r = 0
    if r > 255: r = 255
    if g < 0: g = 0
    if g > 255: g = 255
    if b < 0: b = 0
    if b > 255: b = 255
    
    imgdata[y, x, 0] = r
    imgdata[y, x, 1] = g
    imgdata[y, x, 2] = b
    imgdata[y, x, 3] = 255

    
def clearImg(imgdata):
    for i in xrange(0, imgdata.shape[1]):
        for j in xrange(0, imgdata.shape[2]):
            setpixel(imgdata, i, j, 0, 0, 0)
    

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
