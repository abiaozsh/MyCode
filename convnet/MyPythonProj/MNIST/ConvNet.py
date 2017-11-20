import tensorflow as tf
import numpy as np
from six.moves import xrange

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
    pool_shape = conv.get_shape().as_list()
    reshape = tf.reshape(conv, [pool_shape[0], pool_shape[1] * pool_shape[2] * pool_shape[3]])
    return reshape

#tf.nn.conv2d_transpose(value, filter, output_shape, strides, padding='SAME', name=None)

def ConvLayer(inputT,filterSize,outDepth,convStride,poolSize, loadFromFile = None, noChange=False, isRelu = True):
    
    inDepth = int(inputT.shape[3])
    
    if loadFromFile:
        biasarray = np.ndarray([outDepth], np.float32)

        for i in xrange(0,outDepth):
            line = loadFromFile.readline();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)


        warray = np.ndarray([filterSize, filterSize, inDepth, outDepth], np.float32)

      
        for j in xrange(0,outDepth):
            for x in xrange(0,filterSize):
                for y in xrange(0,filterSize):
                    for i in xrange(0,inDepth):
                        line = loadFromFile.readline();
                        warray[x,y,i,j] = float(line)

        weights = tf.constant(warray)

        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outDepth], dtype=tf.float32))
        #biasarray = np.ndarray([outDepth], np.float32)
        #for i in xrange(0,outDepth):
        #    biasarray[i] = float((i+1)*0.1)
        #biases = tf.constant(biasarray)
        
        weights = tf.truncated_normal([filterSize, filterSize, inDepth, outDepth], stddev=0.1, dtype=tf.float32)


    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
       
    
    _conv = tf.nn.conv2d(inputT,weights,strides=[1, convStride, convStride, 1],padding='SAME')
    # Bias and rectified linear non-linearity.
    #tf.zeros([32], dtype=data_type())

    if isRelu:
        _relu = tf.nn.relu(tf.nn.bias_add(_conv, biases))
    else:
        _relu = tf.nn.bias_add(_conv, biases)
    # Max pooling. The kernel size spec {ksize} also follows the layout of
    # the data. Here we have a pooling window of 2, and a stride of 2.
    if poolSize>1:
        _out = tf.nn.max_pool(_relu,ksize=[1, poolSize, poolSize, 1],strides=[1, poolSize, poolSize, 1],padding='SAME')
    else:
        _out = _relu
        
    def saver(sess,saveToFile):
        biasarray = sess.run(biases)

        for i in xrange(0,outDepth):
            val = biasarray[i]
            saveToFile.write(str(val)+"\n");

        warray = sess.run(weights)

        for j in xrange(0,outDepth):
            for x in xrange(0,filterSize):
                for y in xrange(0,filterSize):
                    for i in xrange(0,inDepth):
                        val = warray[x,y,i,j]
                        saveToFile.write(str(val)+"\n");


    return _out,saver

def FCLayer(inputT,outCnt,isRelu = False, loadFromFile = None, noChange=False):#inCnt,
    inCnt = int(inputT.shape[1])
    _input=inCnt
    _output=outCnt

    if loadFromFile:
        biasarray = np.ndarray([outCnt], np.float32)

        for i in xrange(0,outCnt):
            line = loadFromFile.readline();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)


        warray = np.ndarray([inCnt,outCnt], np.float32)

        for j in xrange(0,outCnt):
            for i in xrange(0,inCnt):
                line = loadFromFile.readline();
                warray[i,j] = float(line)

        weights = tf.constant(warray)

        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outCnt], dtype=tf.float32))
        #biasarray = np.ndarray([outCnt], np.float32)
        #for i in xrange(0,outCnt):
        #    biasarray[i] = float((i+1)*0.1)
        #biases = tf.constant(biasarray)
        
        weights = tf.truncated_normal([inCnt,outCnt], stddev=0.1, dtype=tf.float32)


    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
   
    layer = tf.nn.bias_add(tf.matmul(inputT, weights), biases)
    
    def saver(sess,saveToFile):

        biasarray = sess.run(biases)

        for i in xrange(0,outCnt):
            val = biasarray[i]
            saveToFile.write(str(val)+"\n");

        warray = sess.run(weights)

        for j in xrange(0,outCnt):
            for i in xrange(0,inCnt):
                val = warray[i,j]
                saveToFile.write(str(val)+"\n");

        return
        
    
    if isRelu:
        return tf.nn.relu(layer),saver
    else:
        return layer,saver

#def init(self):
#    self.biasarray[0] = 0.1
#    self.biasarray[1] = 0.12
#    self.biasarray[2] = 0.1
#    self.biasarray[3] = 0.1
#    return
#sess.run(tf.assign(fc1_biases,biasarray))
#print(sess.run(fc1_biases))
    