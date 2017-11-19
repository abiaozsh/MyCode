import tensorflow as tf
import numpy as np

#  hidden = tf.nn.dropout(hidden, 0.5, seed=SEED)

def Conv2FC_Reshape(conv):
    pool_shape = conv.get_shape().as_list()
    reshape = tf.reshape(conv, [pool_shape[0], pool_shape[1] * pool_shape[2] * pool_shape[3]])
    return reshape

#tf.nn.conv2d_transpose(value, filter, output_shape, strides, padding='SAME', name=None)

def ConvLayer(inputT,filterSize,inDepth,outDepth,convStride,poolSize, loadFromFile = None, noChange=False):
    

    if loadFromFile:
        biasarray = np.ndarray([outDepth], np.float32)

        for i in range(0,outDepth):
            line = loadFromFile.next();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)


        warray = np.ndarray([filterSize, filterSize, inDepth, outDepth], np.float32)

        for x in range(0,filterSize):
            for y in range(0,filterSize):
                for i in range(0,inDepth):
                    for j in range(0,outDepth):
                        line = loadFromFile.next();
                        warray[x,y,i,j] = float(line)

        weights = tf.constant(warray)

        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outDepth], dtype=tf.float32))
        weights = tf.truncated_normal([filterSize, filterSize, inDepth, outDepth], stddev=0.1, dtype=tf.float32)


    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
       
    
    _conv = tf.nn.conv2d(inputT,weights,strides=[1, convStride, convStride, 1],padding='SAME')
    # Bias and rectified linear non-linearity.
    #tf.zeros([32], dtype=data_type())

    _relu = tf.nn.relu(tf.nn.bias_add(_conv, biases))
    # Max pooling. The kernel size spec {ksize} also follows the layout of
    # the data. Here we have a pooling window of 2, and a stride of 2.
    _pool = tf.nn.max_pool(_relu,ksize=[1, poolSize, poolSize, 1],strides=[1, poolSize, poolSize, 1],padding='SAME')
    
    def saver(sess):
        #_arrW = sess.run(weights)
        #_arrB = sess.run(biases)
        bbb = sess + biases + weights
        return

    return _pool,saver

def FCLayer(inputT,outCnt,isRelu = False, loadFromFile = None, noChange=False):#inCnt,
    inCnt = int(inputT.shape[1])
    _input=inCnt
    _output=outCnt

    if loadFromFile:
        biasarray = np.ndarray([outCnt], np.float32)

        for i in range(0,outCnt):
            line = loadFromFile.next();
            biasarray[i] = float(line)

        biases = tf.constant(biasarray)


        warray = np.ndarray([inCnt,outCnt], np.float32)

        for i in range(0,inCnt):
            for j in range(0,outCnt):
                line = loadFromFile.next();
                warray[i,j] = float(line)

        weights = tf.constant(warray)

        
    else:
        biases = tf.Variable(tf.constant(0.1, shape=[outCnt], dtype=tf.float32))
        weights = tf.truncated_normal([inCnt,outCnt], stddev=0.1, dtype=tf.float32)


    if not noChange:
        weights = tf.Variable(weights)
        biases = tf.Variable(biases)
   
    layer = tf.nn.bias_add(tf.matmul(inputT, weights), biases)
    
    def saver(sess):
        #_arrW = sess.run(weights)
        #_arrB = sess.run(biases)
        bbb = sess + biases + weights
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
    