from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import gzip
import os
import sys
import time

import numpy as np
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet

from PIL import Image

ind1 = np.ndarray([1,6, 6, 2], np.float32)
for i in xrange(0, 6):
    for j in xrange(0, 6):
        ind1[0,i, j, 0] = (i+1) * 6 + (j+1);
        ind1[0,i, j, 1] = (i+1) * 6 + (j+1) + 0.5;
        
inputLayer = tf.placeholder(tf.float32, shape=(1,6, 6, 2))

testfile = ConvNet.openEmptyFileR('conv.txt')
conv1, conv1save = ConvNet.ConvLayer(inputLayer, filterSize=3, outDepth=4, convStride=1, poolSize=0, loadFromFile=testfile,isRelu = False)
reshape = ConvNet.Conv2FC_Reshape(conv1)
reshape2 = ConvNet.Conv2FC_Reshape(inputLayer)
fc1,fc1save = ConvNet.FCLayer(reshape, 8, isRelu = False, loadFromFile=testfile)
if testfile:testfile.close()

with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    
    _out0 = sess.run(reshape2, feed_dict={inputLayer: ind1})
    _out1 = sess.run(reshape, feed_dict={inputLayer: ind1})
    _out2 = sess.run(fc1, feed_dict={inputLayer: ind1})
    
    
    print(_out0)
   
    print(_out1)
    
    print(_out2)

    testfile = ConvNet.openEmptyFileW('conv.txt')
    conv1save(sess, testfile)
    fc1save(sess, testfile)
    if testfile:testfile.flush(), testfile.close()   
