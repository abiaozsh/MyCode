import tensorflow as tf
import numpy as np
from six.moves import xrange

import ConvNet #我写的tensorflow的壳，不用太在意，主要是打包层，和保存层的参数，tensorflow有自己的序列化方法，不过和我以前用的框架不通用所以我自己写了一个
import MNISTDataLargeSet

BATCH_SIZE = 256

#用来验证的数据，和训练数据一样，也可以用不同的数据验证

#输入层
inputlayer = tf.placeholder(tf.float32, [BATCH_SIZE, MNISTDataLargeSet.IMAGE_H, MNISTDataLargeSet.IMAGE_W, MNISTDataLargeSet.IMAGE_CHANNEL])
testOne = tf.placeholder(tf.float32, [BATCH_SIZE, MNISTDataLargeSet.IMAGE_H, MNISTDataLargeSet.IMAGE_W, MNISTDataLargeSet.IMAGE_CHANNEL])

#预期的数据
labels_node = tf.placeholder(tf.int64, shape=(BATCH_SIZE))

#网络定义
plist = []
loadFromFile = ConvNet.openEmptyFileR('MNISTLargeSet.txt')#从文件中加载已训练的，文件不存在的话默认会随机初始化网络
cv0 = ConvNet.addlist(plist,ConvNet.Conv(inDepth = MNISTDataLargeSet.IMAGE_CHANNEL,outDepth = 16,filterSize = 5,loadFromFile = loadFromFile))
cv1 = ConvNet.addlist(plist,ConvNet.Conv(inDepth = 16,outDepth = 32,filterSize = 5,loadFromFile = loadFromFile))
fc0 = ConvNet.addlist(plist,ConvNet.FC(inDepth = 7*7*32,outDepth = 64,loadFromFile = loadFromFile))
fc1 = ConvNet.addlist(plist,ConvNet.FC(inDepth = 64,outDepth = 10,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()   

def net(inputT):
    _ret = cv0.getLayer(inputT, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = cv1.getLayer(_ret, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = fc0.getLayer(_ret, isRelu=True, fixed = False)
    _ret = fc1.getLayer(_ret, isRelu=False, fixed = False)
    return _ret

fc2 = net(inputlayer)
_net = tf.nn.softmax(fc2)
_test = tf.nn.softmax(net(testOne))

#损失函数
loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(labels=labels_node, logits=fc2))  #这个loss是用来处理回归的情况，现在是分类所以不用
#loss = tf.reduce_sum(tf.square(fc2 - finaldata))

#训练器 可以试试不同的训练器，看对训练结果的影响
#optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  
# tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')  
optimizer = tf.train.GradientDescentOptimizer(0.001).minimize(loss)

def error_rate(predictions, labels):
    """Return the error rate based on dense predictions and sparse labels."""
    return np.sum(np.argmax(predictions, 1) == labels)/BATCH_SIZE

def train():
    #with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
    with tf.Session() as sess:
        #初始化参数
        sess.run(tf.global_variables_initializer())


        for j in xrange(0, 1000):
            #打印当前网络的输出值
            accurate = 0
            testData = MNISTDataLargeSet.extract_data(BATCH_SIZE)
            testLabel = MNISTDataLargeSet.extract_label(BATCH_SIZE)
            lbl = sess.run(_test, feed_dict={testOne:testData})
            accurate = error_rate(lbl,testLabel)
            
            #执行训练
            totalLoss = 0.0
            for _i in xrange(0,100):
                trainData = MNISTDataLargeSet.extract_data(BATCH_SIZE)
                trainLabel = MNISTDataLargeSet.extract_label(BATCH_SIZE)
                _,_loss = sess.run([optimizer,loss], feed_dict={labels_node: trainLabel, inputlayer: trainData})
                totalLoss = totalLoss + _loss
            
            print(j,"accu:",accurate,"loss",totalLoss)
            
            #保存已训练的网络
            Saver = []
            for item in plist:
                Saver.append(item.getSaver(sess))
                
            saveToFile = ConvNet.openEmptyFileW("MNISTLargeSet.txt")
            for item in Saver:
                item(saveToFile)
            saveToFile.flush();saveToFile.close()
        

train()