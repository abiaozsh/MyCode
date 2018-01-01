import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time
import os

from six.moves import xrange

BATCH_SIZE = 16

IMAGE_W = 192
IMAGE_H = 256

Z_DIM = 128
IMAGE_CHANNEL = 3
LR = 0.00001         # Learning rate

file_index = 0
content_index = 0
filePath = "F:\\MNIST\\celebaBetter\\"
bytestream = open(filePath + str(file_index)+".bin","br")
def extract_data():
    global file_index
    global content_index
    global bytestream

    content_index = content_index + BATCH_SIZE
    if content_index>=4096:#202599
        file_index = file_index + 1
        if file_index >= 31:
            file_index = 0
        bytestream.close()
        bytestream = open(filePath + str(file_index)+".bin","br")
        content_index = 0

    buf = bytestream.read(BATCH_SIZE * IMAGE_H * IMAGE_W * IMAGE_CHANNEL)
    data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
    data = (data) / 256.0 - 0.5
    data = data.reshape(BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL)
    return data


# The implementation of PixelShuffler
def pixelShuffler(inputs, scale=2):
    size = tf.shape(inputs)
    batch_size = size[0]
    h = size[1]
    w = size[2]
    c = inputs.get_shape().as_list()[-1]

    # Get the target channel size
    channel_target = c // (scale * scale)
    channel_factor = c // channel_target

    shape_1 = [batch_size, h, w, channel_factor // scale, channel_factor // scale]
    shape_2 = [batch_size, h * scale, w * scale, 1]

    # Reshape and transpose for periodic shuffling for each channel
    input_split = tf.split(inputs, channel_target, axis=3)
    output = tf.concat([phaseShift(x, scale, shape_1, shape_2) for x in input_split], axis=3)

    return output


def phaseShift(inputs, scale, shape_1, shape_2):
    # Tackle the condition when the batch is None
    X = tf.reshape(inputs, shape_1)
    X = tf.transpose(X, [0, 1, 3, 2, 4])

    return tf.reshape(X, shape_2)


def train():

    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])

    net = pixelShuffler(images,scale=1)
    
    
    with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
        init = tf.global_variables_initializer()  
        sess.run(init)

        loadedimage = extract_data()
        sample = sess.run(net, feed_dict = {images:loadedimage})
        
        ConvNet.saveImages(loadedimage, [2,8], 'sample_in.png')

        ConvNet.saveImages(sample, [2,8], 'sample_out.png')

    
train()
