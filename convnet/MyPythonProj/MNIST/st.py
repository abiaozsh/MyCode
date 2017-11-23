from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import numpy as np
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import tensorflow.layers
import ConvNet

with tf.device("/cpu:0"):
    ind1 = np.ndarray([1, 4, 4, 2], np.float32)
    for i in xrange(0, 4):
        for j in xrange(0, 4):
            ind1[0,i, j, 0] = (i+1) * 4 + (j+1);
            ind1[0,i, j, 1] = (i+1) * 4 + (j+1) + 0.5;
            
    inputLayer = tf.placeholder(tf.float32, shape=(1,4, 4, 2))
    
    weights = tf.truncated_normal([3, 3, 4, 2], stddev=0.1, dtype=tf.float32)
    
    _conv = tf.nn.conv2d_transpose(inputLayer, weights, output_shape=[1,8,8,4], strides=[1, 2, 2, 1], padding="SAME")
    tensorflow.layers.Convolution2DTranspose
    
    with tf.Session() as sess:
    
            sess.run(tf.global_variables_initializer())
            
            sess.run(_conv, feed_dict={inputLayer: ind1})
            print()
            print()
