from __future__ import print_function
from __future__ import absolute_import
from __future__ import division
import sys

import numpy as np
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet

IMAGE_SIZEH = 32
IMAGE_SIZEW = 32
NUM_CHANNELS = 3
BATCH_SIZE = 64

def extract_data(filename, num_images):
    with open(filename,"br") as bytestream:
        buf = bytestream.read(num_images * IMAGE_SIZEH * IMAGE_SIZEW * NUM_CHANNELS)
        data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
        data = (data) / 255.0 - 0.5
        data = data.reshape(num_images, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS)
        return data

def saveimg(filename,wholeData,idx,scale = 255,bias = 128):
    data = ConvNet.newImage(wholeData.shape[1],wholeData.shape[2])
    #clearImg(data,0)
    for i in xrange(0,wholeData.shape[1]):
        for j in xrange(0,wholeData.shape[2]):
            r = wholeData[idx,j,i,0] * scale + bias
            g = wholeData[idx,j,i,1] * scale + bias
            b = wholeData[idx,j,i,2] * scale + bias
            ConvNet.setpixel(data,i,j,r,g,b)
    #lbl = wholeData[idx]
    ConvNet.saveImg(data, filename)

CifarData = extract_data("E:\\MNIST\\cifar-10-batches-bin\\HWC.bin",60000)

inputLayer = tf.placeholder(tf.float32,shape=(BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS))
finaldata = tf.placeholder(tf.float32, shape=(BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS))

inputData = np.ndarray([BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS], np.float32)
verifydata = np.ndarray([BATCH_SIZE, IMAGE_SIZEH, IMAGE_SIZEW, NUM_CHANNELS], np.float32)

testfile = ConvNet.openEmptyFileR('cifar.txt')
conv1,conv1save = ConvNet.ConvLayer(inputLayer,filterSize = 5,outDepth = 32,convStride = 1,poolSize = 2,loadFromFile=testfile)
conv2,conv2save = ConvNet.ConvLayer(conv1,filterSize = 5,outDepth = 32,convStride = 1,poolSize = 2,loadFromFile=testfile)
conv3,conv3save = ConvNet.ConvLayer(conv2,filterSize = 5,outDepth = 64,convStride = 1,poolSize = 2,loadFromFile=testfile)
reshape = ConvNet.Conv2FC_Reshape(conv3)
#fc1,fc1saver = ConvNet.FCLayer(reshape,2048,isRelu = False,loadFromFile=testfile)
fc2,fc1saver = ConvNet.FCLayer(reshape,2048,isRelu = True,loadFromFile=testfile)

deshape = ConvNet.FC2Conv_Reshape(fc2,4,4,128)
uconv1,uconv1save = ConvNet.DeConvLayer(deshape,filterSize=5,output_shape=[BATCH_SIZE,8,8,64],convStride = 2,poolSize = 0, loadFromFile = None, noChange=False, isRelu = True,padding = True)
uconv2,uconv2save = ConvNet.DeConvLayer(uconv1,filterSize=5,output_shape=[BATCH_SIZE,16,16,64],convStride = 2,poolSize = 0, loadFromFile = None, noChange=False, isRelu = True,padding = True)
uconv3,uconv3save = ConvNet.DeConvLayer(uconv2,filterSize=5,output_shape=[BATCH_SIZE,32,32,3],convStride = 2,poolSize = 0, loadFromFile = None, noChange=False, isRelu = False,padding = True)
regeneratedImg = uconv3

if testfile:testfile.close()   

loss = tf.reduce_sum(tf.square(regeneratedImg - finaldata))
optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')
#optimizer = tf.train.GradientDescentOptimizer(0.00001).minimize(loss)



def train():
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())

        saveimg("base0.bmp",CifarData,0)
        saveimg("base1.bmp",CifarData,1)
        saveimg("base2.bmp",CifarData,2)
        saveimg("base3.bmp",CifarData,3)
        saveimg("base4.bmp",CifarData,59110)
        saveimg("base5.bmp",CifarData,59111)
        saveimg("base6.bmp",CifarData,59112)
        saveimg("base7.bmp",CifarData,59113)

        verifydata[0] = CifarData[0]
        verifydata[1] = CifarData[1]
        verifydata[2] = CifarData[2]
        verifydata[3] = CifarData[3]
        verifydata[4] = CifarData[59110]
        verifydata[5] = CifarData[59111]
        verifydata[6] = CifarData[59112]
        verifydata[7] = CifarData[59113]
  
        for j in xrange(0, 100):
            
            for k in xrange(0,10):#train times
                print(str(k)+' ',end='')
                sys.stdout.flush()
                for i in xrange(0, 800):#train range 800
                    for dj in xrange(0, BATCH_SIZE):
                        inputData[dj] = CifarData[dj+i*BATCH_SIZE]
                    sess.run(optimizer, feed_dict={finaldata: inputData, inputLayer: inputData})

            print()
            print(j)
            
            _out = sess.run(regeneratedImg, feed_dict={inputLayer:verifydata})
            saveimg(str(j)+"0.bmp",_out,0)
            saveimg(str(j)+"1.bmp",_out,1)
            saveimg(str(j)+"2.bmp",_out,2)
            saveimg(str(j)+"3.bmp",_out,3)
            saveimg(str(j)+"4.bmp",_out,4)
            saveimg(str(j)+"5.bmp",_out,5)
            saveimg(str(j)+"6.bmp",_out,6)
            saveimg(str(j)+"7.bmp",_out,7)

#         testfile = ConvNet.openEmptyFileW('cifar.txt')
#         conv1save(sess,testfile)
#         conv2save(sess,testfile)
#         conv3save(sess,testfile)
#         fc1saver(sess,testfile)
#         fc2saver(sess,testfile)
#         if testfile:testfile.flush(),testfile.close()   
         
def test():
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        _out = sess.run(regeneratedImg, feed_dict={inputLayer:verifydata})
        
        saveimg("0.bmp",_out,0)
        saveimg("1.bmp",_out,1)

train()
#test()

#saveimg("0.bmp", data, 0)

