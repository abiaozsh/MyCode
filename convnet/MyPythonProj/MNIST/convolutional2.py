# Copyright 2015 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

"""Simple, end-to-end, LeNet-5-like convolutional MNIST model example.

This should achieve a test error of 0.7%. Please keep this model as simple and
linear as possible, it is meant as a tutorial for simple convolutional models.
Run with --self_test on the command line to execute a short self-test.
"""
from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import gzip
import os
import sys
import time

import numpy
from six.moves import urllib
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet

SOURCE_URL = 'http://yann.lecun.com/exdb/mnist/'
WORK_DIRECTORY = 'data'
IMAGE_SIZE = 28
NUM_CHANNELS = 1
PIXEL_DEPTH = 255
NUM_LABELS = 10
SEED = 66478  # Set to None for random seed.
BATCH_SIZE = 64
VALIDATION_SIZE = 64  # Size of the validation set.
NUM_EPOCHS = 10
EVAL_FREQUENCY = 100  # Number of steps between evaluations.


FLAGS = None


def data_type():
    return tf.float32


def maybe_download(filename):
    """Download the data from Yann's website, unless it's already here."""
    if not tf.gfile.Exists(WORK_DIRECTORY):
        tf.gfile.MakeDirs(WORK_DIRECTORY)
    filepath = os.path.join(WORK_DIRECTORY, filename)
    if not tf.gfile.Exists(filepath):
        filepath, _ = urllib.request.urlretrieve(SOURCE_URL + filename, filepath)
        with tf.gfile.GFile(filepath) as f:
            size = f.size()
        print('Successfully downloaded', filename, size, 'bytes.')
    return filepath


def extract_data(filename, num_images):
    """Extract the images into a 4D tensor [image index, y, x, channels].
    
    Values are rescaled from [0, 255] down to [-0.5, 0.5].
    """
    print('Extracting', filename)
    with gzip.open(filename) as bytestream:
        bytestream.read(16)
        buf = bytestream.read(IMAGE_SIZE * IMAGE_SIZE * num_images * NUM_CHANNELS)
        data = numpy.frombuffer(buf, dtype=numpy.uint8).astype(numpy.float32)
        data = (data - (PIXEL_DEPTH / 2.0)) / PIXEL_DEPTH
        data = data.reshape(num_images, IMAGE_SIZE, IMAGE_SIZE, NUM_CHANNELS)
        return data
        

def extract_labels(filename, num_images):
    """Extract the labels into a vector of int64 label IDs."""
    print('Extracting', filename)
    with gzip.open(filename) as bytestream:
        bytestream.read(8)
        buf = bytestream.read(1 * num_images)
        labels = numpy.frombuffer(buf, dtype=numpy.uint8).astype(numpy.int64)
    return labels



def error_rate(predictions, labels):
    """Return the error rate based on dense predictions and sparse labels."""
    return str(numpy.sum(numpy.argmax(predictions, 1) == labels)) + "," + str(predictions.shape[0])


# Get the data.
train_data_filename = maybe_download('train-images-idx3-ubyte.gz')
train_labels_filename = maybe_download('train-labels-idx1-ubyte.gz')
test_data_filename = maybe_download('t10k-images-idx3-ubyte.gz')
test_labels_filename = maybe_download('t10k-labels-idx1-ubyte.gz')

# Extract it into numpy arrays.
train_data = extract_data(train_data_filename, 60000)
train_labels = extract_labels(train_labels_filename, 60000)
test_data = extract_data(test_data_filename, 10000)
test_labels = extract_labels(test_labels_filename, 10000)

# Generate a validation set.
validation_data = train_data[:VALIDATION_SIZE, ...]
validation_labels = train_labels[:VALIDATION_SIZE]
train_data = train_data[VALIDATION_SIZE:, ...]
train_labels = train_labels[VALIDATION_SIZE:]
num_epochs = NUM_EPOCHS

train_size = train_labels.shape[0]

# This is where training samples and labels are fed to the graph.
# These placeholder nodes will be fed a batch of training data at each
# training step using the {feed_dict} argument to the Run() call below.
train_data_node = tf.placeholder(
    data_type(),
    shape=(BATCH_SIZE, IMAGE_SIZE, IMAGE_SIZE, NUM_CHANNELS))

# We will replicate the model structure for the training subgraph, as well
# as the evaluation subgraphs, while sharing the trainable parameters.
#def model(data):



conv1,xx = ConvNet.ConvLayer(train_data_node,filterSize = 5,inDepth = NUM_CHANNELS,outDepth = 32,convStride = 1,poolSize = 2)
conv2,xx = ConvNet.ConvLayer(conv1,filterSize = 5,inDepth = 32,outDepth = 64,convStride = 1,poolSize = 2)
reshape = ConvNet.Conv2FC_Reshape(conv2)
fc1,xx = ConvNet.FCLayer(reshape, 512, isRelu = True)
fc2,xx = ConvNet.FCLayer(fc1, NUM_LABELS)
train_prediction = tf.nn.softmax(fc2)

train_labels_node = tf.placeholder(tf.int64, shape=(BATCH_SIZE,))
loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(labels=train_labels_node, logits=fc2))

# L2 regularization for the fully connected parameters.
#regularizers = (tf.nn.l2_loss(fc1_weights) + tf.nn.l2_loss(fc1_biases) +
#                tf.nn.l2_loss(fc2_weights) + tf.nn.l2_loss(fc2_biases))
# Add the regularization term to the loss.
#loss += 5e-4 * regularizers

# Optimizer: set up a variable that's incremented once per batch and
# controls the learning rate decay.
batch = tf.Variable(0, dtype=data_type())
# Decay once per epoch, using an exponential schedule starting at 0.01.
learning_rate = tf.train.exponential_decay(
    0.01,                # Base learning rate.
    batch * BATCH_SIZE,  # Current index into the dataset.
    train_size,          # Decay step.
    0.95,                # Decay rate.
    staircase=True)
# Use simple momentum for the optimization.
optimizer = tf.train.MomentumOptimizer(learning_rate,
                                       0.9).minimize(loss,
                                                     global_step=batch)

# Predictions for the current training minibatch.

# Predictions for the test and validation, which we'll compute less often.
#eval_prediction = tf.nn.softmax(model(eval_data))

# Small utility function to evaluate a dataset by feeding batches of data to
# {eval_data} and pulling the results from {eval_predictions}.
# Saves memory and enables this to run on smaller GPUs.
def eval_in_batches(data, sess):
    predictions = sess.run(     train_prediction,          feed_dict={train_data_node: data})
    return predictions

# Create a local session to run the training.
start_time = time.time()
with tf.Session() as sess:
    # Run all the initializers to prepare the trainable parameters.
    tf.global_variables_initializer().run()
    print('Initialized!')
    # Loop through training steps.
    for step in xrange(int(num_epochs * train_size) // BATCH_SIZE):
        # Compute the offset of the current minibatch in the data.
        # Note that we could use better randomization across epochs.
        offset = (step * BATCH_SIZE) % (train_size - BATCH_SIZE)
        batch_data = train_data[offset:(offset + BATCH_SIZE), ...]
        batch_labels = train_labels[offset:(offset + BATCH_SIZE)]
        # This dictionary maps the batch data (as a numpy array) to the
        # node in the graph it should be fed to.
        feed_dict = {train_data_node: batch_data,
                     train_labels_node: batch_labels}
        # Run the optimizer to update weights.
        sess.run(optimizer, feed_dict=feed_dict)
        # print some extra information once reach the evaluation frequency
        if step % EVAL_FREQUENCY == 0:
            # fetch some extra nodes' data
            l, lr, predictions = sess.run([loss, learning_rate, train_prediction],
                                          feed_dict=feed_dict)
            elapsed_time = time.time() - start_time
            start_time = time.time()
            print('Step %d (epoch %.2f), %.1f ms' %
                  (step, float(step) * BATCH_SIZE / train_size,
                   1000 * elapsed_time / EVAL_FREQUENCY))
            print(elapsed_time)
            print('Minibatch loss: %.3f, learning rate: %.6f' % (l, lr))
            print("Minibatch error: " + error_rate(predictions, batch_labels))
            print("Validation error: " + error_rate(eval_in_batches(validation_data, sess), validation_labels))
            sys.stdout.flush()
    # Finally print the result!
    test_error = error_rate(eval_in_batches(test_data, sess), test_labels)
    print('Test error: %.1f%%' % test_error)
    if FLAGS.self_test:
        print('test_error', test_error)
        assert test_error == 0.0, 'expected 0.0 test_error, got %.2f' % (
            test_error,)
    
