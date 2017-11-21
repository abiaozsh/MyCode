from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import time

import numpy
from six.moves import xrange  # pylint: disable=redefined-builtin
import tensorflow as tf
import ConvNet

IMAGE_SIZE = 28
NUM_CHANNELS = 1
PIXEL_DEPTH = 255
NUM_LABELS = 10
BATCH_SIZE = 64
VALIDATION_SIZE = 64  # Size of the validation set.
NUM_EPOCHS = 10
EVAL_FREQUENCY = 10  # Number of steps between evaluations.


FLAGS = None


def extract_data(filename, num_images):
    """Extract the images into a 4D tensor [image index, y, x, channels].
    
    """
    print('Extracting', filename)
    with open(filename,"br") as bytestream:
        bytestream.read(16)
        buf = bytestream.read(IMAGE_SIZE * IMAGE_SIZE * num_images * NUM_CHANNELS)
        data = numpy.frombuffer(buf, dtype=numpy.uint8).astype(numpy.float32)
        data = (data) / PIXEL_DEPTH
        data = data.reshape(num_images, IMAGE_SIZE, IMAGE_SIZE, NUM_CHANNELS)
        return data
        

def extract_labels(filename, num_images):
    """Extract the labels into a vector of int64 label IDs."""
    print('Extracting', filename)
    with open(filename,"br") as bytestream:
        bytestream.read(8)
        buf = bytestream.read(num_images)
        labels = numpy.frombuffer(buf, dtype=numpy.uint8).astype(numpy.int64)
    return labels



def error_rate(predictions, labels):
    """Return the error rate based on dense predictions and sparse labels."""
    return str(numpy.sum(numpy.argmax(predictions, 1) == labels))


# Extract it into numpy arrays.
train_data = extract_data('E:\\MNIST\\train-images.idx3-ubyte', 60000)
train_labels = extract_labels('E:\\MNIST\\train-labels.idx1-ubyte', 60000)
test_data = extract_data('E:\\MNIST\\t10k-images.idx3-ubyte', 10000)
test_labels = extract_labels('E:\\MNIST\\t10k-labels.idx1-ubyte', 10000)

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

# We will replicate the model structure for the training subgraph, as well
# as the evaluation subgraphs, while sharing the trainable parameters.
#def model(data):

testfile = ConvNet.openEmptyFileR('conv.txt')
inputLayer = tf.placeholder(tf.float32,shape=(BATCH_SIZE, IMAGE_SIZE, IMAGE_SIZE, NUM_CHANNELS))
conv1,conv1save = ConvNet.ConvLayer(inputLayer,filterSize = 5,outDepth = 32,convStride = 1,poolSize = 2,loadFromFile=testfile)
conv2,conv2save = ConvNet.ConvLayer(conv1,filterSize = 5,outDepth = 64,convStride = 1,poolSize = 2,loadFromFile=testfile)
reshape = ConvNet.Conv2FC_Reshape(conv2)
fc1,fc1save = ConvNet.FCLayer(reshape, 512, isRelu = True,loadFromFile=testfile)
fc2,fc2save = ConvNet.FCLayer(fc1, NUM_LABELS,loadFromFile=testfile)
train_prediction = tf.nn.softmax(fc2)
labels_node = tf.placeholder(tf.int64, shape=(BATCH_SIZE,))
if testfile:testfile.close()   


def eval_in_batches(data, sess):
    predictions = sess.run(train_prediction, feed_dict={inputLayer: data})
    return predictions

def train():
    loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(labels=labels_node, logits=fc2))
    batch = tf.Variable(0, dtype=tf.float32)
    learning_rate = tf.train.exponential_decay(
        0.01,                # Base learning rate.
        batch * BATCH_SIZE,  # Current index into the dataset.
        train_size,          # Decay step.
        0.95,                # Decay rate.
        staircase=True)
    # Use simple momentum for the optimization.
    optimizer = tf.train.MomentumOptimizer(learning_rate,0.9).minimize(loss,global_step=batch)
    # Create a local session to run the training.
    start_time = time.time()
    with tf.Session() as sess:
        # Run all the initializers to prepare the trainable parameters.
        tf.global_variables_initializer().run()
        print('Initialized!')
        # Loop through training steps.
        for step in xrange(0,100):
            # Compute the offset of the current minibatch in the data.
            # Note that we could use better randomization across epochs.
            offset = (step * BATCH_SIZE) % (train_size - BATCH_SIZE)
            batch_data = train_data[offset:(offset + BATCH_SIZE), ...]
            batch_labels = train_labels[offset:(offset + BATCH_SIZE)]
            # This dictionary maps the batch data (as a numpy array) to the
            # node in the graph it should be fed to.
            feed_dict = {inputLayer: batch_data,labels_node: batch_labels}
            # Run the optimizer to update weights.
            sess.run(optimizer, feed_dict=feed_dict)
            # print some extra information once reach the evaluation frequency
            if step % EVAL_FREQUENCY == 0:
                # fetch some extra nodes' data
                l, predictions = sess.run([loss, train_prediction],
                                              feed_dict=feed_dict)
                elapsed_time = time.time() - start_time
                start_time = time.time()
                print(elapsed_time,l,  error_rate(predictions, batch_labels), error_rate(eval_in_batches(validation_data, sess), validation_labels))
    
        print("save start")
        testfile = ConvNet.openEmptyFileW('conv.txt')
        conv1save(sess,testfile)
        conv2save(sess,testfile)
        fc1save(sess,testfile)
        fc2save(sess,testfile)
        if testfile:testfile.flush(),testfile.close()   
        print("save done")
    
def test():

    with tf.Session() as sess:
        # Run all the initializers to prepare the trainable parameters.
        tf.global_variables_initializer().run()
        print('Initialized!')
        # Loop through training steps.

        # Compute the offset of the current minibatch in the data.
        # Note that we could use better randomization across epochs.
        offset = 601
        batch_data = train_data[offset:(offset + BATCH_SIZE), ...]
        batch_labels = train_labels[offset:(offset + BATCH_SIZE)]
        # This dictionary maps the batch data (as a numpy array) to the
        # node in the graph it should be fed to.
        feed_dict = {inputLayer: batch_data,labels_node: batch_labels}
        # Run the optimizer to update weights.

        # fetch some extra nodes' data
        predictions = sess.run(train_prediction,feed_dict=feed_dict)

        print(error_rate(predictions, batch_labels), error_rate(eval_in_batches(validation_data, sess), validation_labels))

train()
#test()