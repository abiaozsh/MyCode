from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet
import tensorflow.contrib.rnn.BasicLSTMCell



ind1 = np.ndarray([1, 8, 8, 2], np.float32)
for i in xrange(0, 8):
    for j in xrange(0, 8):
        ind1[0,i, j, 0] = (i+1) * 8 + (j+1);
        ind1[0,i, j, 1] = (i+1) * 8 + (j+1) + 0.5;
        
inputLayer = tf.placeholder(tf.float32, shape=(1,8, 8, 2))

testfile = ConvNet.openEmptyFileR('conv.txt')
conv1, conv1save = ConvNet.ConvLayer(inputLayer, filterSize=5, outDepth=4, convStride=1,padding=True, poolSize=2, loadFromFile=testfile)
conv2, conv2save = ConvNet.ConvLayer(conv1, filterSize=5, outDepth=8, convStride=1,padding=True, poolSize=2, loadFromFile=testfile)
reshape = ConvNet.Conv2FC_Reshape(conv2)
fc1,fc1save = ConvNet.FCLayer(reshape, 8, loadFromFile=testfile)
fc2,fc2save = ConvNet.FCLayer(fc1, 2*2*8, loadFromFile=testfile)
deshape = ConvNet.FC2Conv_Reshape(fc2,2,2,8)
uconv1,uconv1save = ConvNet.DeConvLayer(deshape,filterSize=5,output_shape=[1,4,4,4],convStride = 2,loadFromFile = testfile)
uconv2,uconv2save = ConvNet.DeConvLayer(uconv1,filterSize=5,output_shape=[1,8,8,2],convStride = 2,loadFromFile = testfile)

if testfile:testfile.close()


with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    
    ConvNet.outprint(sess.run(inputLayer, feed_dict={inputLayer: ind1}))
    print()
    ConvNet.outprint(sess.run(conv1, feed_dict={inputLayer: ind1}))
    print()
    ConvNet.outprint(sess.run(conv2, feed_dict={inputLayer: ind1}))
    print()
    ConvNet.outprint(sess.run(fc1, feed_dict={inputLayer: ind1}))
    print()
    ConvNet.outprint(sess.run(fc2, feed_dict={inputLayer: ind1}))
    print()
    ConvNet.outprint(sess.run(uconv1, feed_dict={inputLayer: ind1}))
    print()
    ConvNet.outprint(sess.run(uconv2, feed_dict={inputLayer: ind1}))
    print()


    testfile = ConvNet.openEmptyFileW('conv.txt')
    conv1save(sess, testfile)
    conv2save(sess, testfile)
    fc1save(sess, testfile)
    fc2save(sess, testfile)
    uconv1save(sess, testfile)
    uconv2save(sess, testfile)
    if testfile:testfile.flush(), testfile.close()   


