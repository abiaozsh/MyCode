from __future__ import print_function
from __future__ import absolute_import
from __future__ import division
import sys

import numpy as np
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet

#64,24,32,3
BATCH_SIZE = 64
IMAGE_SIZEH = 24
IMAGE_SIZEW = 32
NUM_CHANNELS = 3

def extract_data(filename, num_images):
    with open(filename,"br") as bytestream:
        buf = bytestream.read(num_images * IMAGE_SIZEH * IMAGE_SIZEW * NUM_CHANNELS)
        data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
        data = (data) / 255.0 - 0.5
        data = data.reshape(num_images, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS)
        return data

CifarData = extract_data("E:\\MNIST\\cifar-10-batches-bin\\3224.bin",60000)

inputData = np.ndarray([BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS], np.float32)
verifydata = np.ndarray([BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS], np.float32)

loadFromFile = ConvNet.openEmptyFileR('cifar.txt')
cv1  = ConvNet.Conv(inDepth = NUM_CHANNELS,outDepth = 32,filterSize = 5, loadFromFile=loadFromFile)#16*16
cv2  = ConvNet.Conv(inDepth = 32,outDepth = 32,filterSize = 5, loadFromFile=loadFromFile)#8*8
cv3  = ConvNet.Conv(inDepth = 32,outDepth = 64,filterSize = 5, loadFromFile=loadFromFile)#4*4
fc2 = ConvNet.FC(inDepth = 3*4*64,outDepth = 3*4*128,loadFromFile = loadFromFile)
dc1 = ConvNet.DeConv(inDepth = 128,outDepth = 64,filterSize = 5,loadFromFile = loadFromFile)
dc2 = ConvNet.DeConv(inDepth = 64,outDepth = 64,filterSize = 5,loadFromFile = loadFromFile)
dc3 = ConvNet.DeConv(inDepth = 64,outDepth = 3,filterSize = 5,loadFromFile = loadFromFile)
if loadFromFile:loadFromFile.close()   


inputLayer = tf.placeholder(tf.float32,shape=(BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS))
finaldata = tf.placeholder(tf.float32, shape=(BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS))

net = cv1.getLayer(inputLayer, convStride = 1, poolSize = 2, isRelu = True, fixed = False)
net = cv2.getLayer(net, convStride = 1, poolSize = 2, isRelu = True, fixed = False)
net = cv3.getLayer(net, convStride = 1, poolSize = 2, isRelu = True, fixed = False)
net = ConvNet.Conv2FC_Reshape(net)
net = fc2.getLayer(net, isRelu = True, fixed = False)
net = ConvNet.FC2Conv_Reshape(net, 3, 4, 128)
net = dc1.getLayer(net, height = 6, width = 8, convStride = 2, isRelu = True, fixed = False)
net = dc2.getLayer(net, height = 12, width = 16, convStride = 2, isRelu = True, fixed = False)
net = dc3.getLayer(net, height = 24, width = 32, convStride = 2, isRelu = False, fixed = False)


regeneratedImg = net



loss = tf.reduce_sum(tf.square(regeneratedImg - finaldata))
optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')
#optimizer = tf.train.GradientDescentOptimizer(0.00001).minimize(loss)



def train():
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())

        verifydata[0] = CifarData[0]
        verifydata[1] = CifarData[1]
        verifydata[2] = CifarData[2]
        verifydata[3] = CifarData[3]
        verifydata[4] = CifarData[59110]
        verifydata[5] = CifarData[59111]
        verifydata[6] = CifarData[59112]
        verifydata[7] = CifarData[59113]

        ConvNet.saveImages(verifydata, [8, 8], 'base.png')
  
        for j in xrange(0, 100):
            
            for k in xrange(0,10):#train times
                print(str(k)+' ',end='')
                sys.stdout.flush()
                for i in xrange(0, 64):#train range 800
                    for dj in xrange(0, BATCH_SIZE):
                        inputData[dj] = CifarData[dj+i*BATCH_SIZE]
                    _,_loss = sess.run([optimizer,loss], feed_dict={finaldata: inputData, inputLayer: inputData})

            print()
            print(j,_loss)
            
            _out = sess.run(regeneratedImg, feed_dict={inputLayer:verifydata})

            ConvNet.saveImages(_out, [8, 8], "test"+str(j)+'.png')

            saveToFile = ConvNet.openEmptyFileW('cifar.txt')
            cv1.save(sess,saveToFile)
            cv2.save(sess,saveToFile)
            cv3.save(sess,saveToFile)
            fc2.save(sess,saveToFile)
            dc1.save(sess,saveToFile)
            dc2.save(sess,saveToFile)
            dc3.save(sess,saveToFile)
            if saveToFile:saveToFile.flush(),saveToFile.close()   

def test():
    verifydata[0] = CifarData[0]
    verifydata[1] = CifarData[1]
    verifydata[2] = CifarData[2]
    verifydata[3] = CifarData[3]
    verifydata[4] = CifarData[59110]
    verifydata[5] = CifarData[59111]
    verifydata[6] = CifarData[59112]
    verifydata[7] = CifarData[59113]

    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        _out = sess.run(regeneratedImg, feed_dict={inputLayer:verifydata})
        
        ConvNet.saveImages(_out, [8, 8], "verify.png")

train()
#test()

#saveimg("0.bmp", data, 0)

