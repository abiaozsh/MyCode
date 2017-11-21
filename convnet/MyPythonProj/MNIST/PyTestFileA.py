import tensorflow as tf
import numpy as np
import ConvNet

# from PIL import Image
# im = Image.open("e:\\aa.jpg")
# im.rotate()
# im = im.resize((224, 224))
# img_raw = im.tobytes() 
# 
# print(img_raw[0],img_raw[1],img_raw[2])
# print(img_raw[3],img_raw[4],img_raw[5])
# im.save("e:\\bb.jpg")

batchSize = 3

SEED = 66478  # Set to None for random seed.

ind1 = np.mat([
    [1.0, 3.0, 1.0, 3.0],
    [2.0, 1.0, 2.0, 1.0],
    [0.0, 0.5, 1.0, 1.5],
    ])

verifydata1 = np.ndarray([batchSize, 4], np.float32)
verifydata1[0] = [1.0, 3.0, 1.0, 3.0]

verifydata2 = np.ndarray([batchSize, 4], np.float32)
verifydata2[0] = [2.0, 1.0, 2.0, 1.0]

verifydata3 = np.ndarray([batchSize, 4], np.float32)
verifydata3[0] = [0.0, 0.5, 1.0, 1.5]

finaldata = tf.placeholder(tf.float32, shape=(batchSize, 4))  # finaldata == inputdata
#inputdata = tf.placeholder(tf.float32, shape=(batchSize, 4))

inputlayer = tf.placeholder(tf.float32, shape=(batchSize, 4))

testfile = ConvNet.openEmptyFileR('test.txt')
fc1,fc1saver = ConvNet.FCLayer(inputlayer,5,isRelu = True,loadFromFile=testfile)
fc2,fc2saver = ConvNet.FCLayer(fc1,4,loadFromFile=testfile)
if testfile:testfile.close()   

# loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(labels=train_labels_node, logits=logits))

loss = tf.reduce_sum(tf.square(fc2 - finaldata))
#optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')
optimizer = tf.train.GradientDescentOptimizer(0.001).minimize(loss)

def train():
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
    
        for j in range(0, 20):
            a1 = sess.run(fc2, feed_dict={inputlayer:verifydata1})
            a2 = sess.run(fc2, feed_dict={inputlayer:verifydata2})
            a3 = sess.run(fc2, feed_dict={inputlayer:verifydata3})
            print(j, a1[0], a2[0], a3[0])
            for i in range(0, 100):
                sess.run(optimizer, feed_dict={finaldata: ind1, inputlayer: ind1})
                
        testfile = ConvNet.openEmptyFileW('test.txt')
        fc1saver(sess,testfile)
        fc2saver(sess,testfile)
        if testfile:testfile.flush(),testfile.close()   
        
def test():
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        a1 = sess.run(fc2, feed_dict={inputlayer:verifydata1})
        a2 = sess.run(fc2, feed_dict={inputlayer:verifydata2})
        a3 = sess.run(fc2, feed_dict={inputlayer:verifydata3})
        print(a1[0], a2[0], a3[0])
train()
#test()