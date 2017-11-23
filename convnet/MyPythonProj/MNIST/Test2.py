from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet

ind1 = np.ndarray([1, 6, 6, 2], np.float32)
for i in xrange(0, 6):
    for j in xrange(0, 6):
        ind1[0,i, j, 0] = (i+1) * 6 + (j+1);
        ind1[0,i, j, 1] = (i+1) * 6 + (j+1) + 0.5;
        
inputLayer = tf.placeholder(tf.float32, shape=(1,6, 6, 2))

testfile = ConvNet.openEmptyFileR('conv.txt')
conv1, conv1save = ConvNet.ConvLayer(inputLayer, filterSize=3, outDepth=4, convStride=1,padding=True, poolSize=2, loadFromFile=testfile,isRelu = False)
reshape = ConvNet.Conv2FC_Reshape(conv1)
fc1,fc1save = ConvNet.FCLayer(reshape, 8, isRelu = False, loadFromFile=testfile)
fc2,fc2save = ConvNet.FCLayer(fc1, 3*3*4, isRelu = False, loadFromFile=testfile)
deshape = ConvNet.FC2Conv_Reshape(fc2,3,3,4)
uconv1,uconv1save = ConvNet.DeConvLayer(deshape,filterSize=3,output_shape=[1,6,6,2],convStride = 2,loadFromFile = testfile, isRelu = False)

if testfile:testfile.close()


with tf.Session() as sess:
    sess.run(tf.global_variables_initializer())
    
    ConvNet.outprint(sess.run(inputLayer, feed_dict={inputLayer: ind1}))
    print()
    print()
    ConvNet.outprint(sess.run(conv1, feed_dict={inputLayer: ind1}))
    print()
    print()
    ConvNet.outprint(sess.run(fc1, feed_dict={inputLayer: ind1}))
    print()
    print()
    ConvNet.outprint(sess.run(fc2, feed_dict={inputLayer: ind1}))
    print()
    print()
    ConvNet.outprint(sess.run(uconv1, feed_dict={inputLayer: ind1}))
    print()


    testfile = ConvNet.openEmptyFileW('conv.txt')
    conv1save(sess, testfile)
    fc1save(sess, testfile)
    fc2save(sess, testfile)
    uconv1save(sess, testfile)
    if testfile:testfile.flush(), testfile.close()   
