import tensorflow as tf
import numpy as np
import ConvNet #我写的tensorflow的壳，不用太在意，主要是打包层，和保存层的参数，tensorflow有自己的序列化方法，不过和我以前用的框架不通用所以我自己写了一个
from six.moves import xrange
#训练数据： 输入网络这3条数据，不断训练
#期望网络在训练后， 输入某一组数据，就能返相应的概率
import MNISTData

BATCH_SIZE = 100

#用来验证的数据，和训练数据一样，也可以用不同的数据验证

#输入层
inputlayer = tf.placeholder(tf.float32, [BATCH_SIZE, MNISTData.IMAGE_H, MNISTData.IMAGE_W, MNISTData.IMAGE_CHANNEL])
testOne = tf.placeholder(tf.float32, [1, MNISTData.IMAGE_H, MNISTData.IMAGE_W, MNISTData.IMAGE_CHANNEL])

#预期的数据
labels_node = tf.placeholder(tf.float32, [BATCH_SIZE,10])

#网络定义
plist = []
loadFromFile = ConvNet.openEmptyFileR('MNIST.txt')#从文件中加载已训练的，文件不存在的话默认会随机初始化网络
cv0 = ConvNet.addlist(plist,ConvNet.Conv(inDepth = MNISTData.IMAGE_CHANNEL,outDepth = 16,filterSize = 5,loadFromFile = loadFromFile))
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
    _ret = tf.nn.softmax(_ret)
    return _ret

_net = net(inputlayer)
_test = net(testOne)

#损失函数
#loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(labels=labels_node, logits=fc2))  #这个loss是用来处理回归的情况，现在是分类所以不用
loss = tf.reduce_sum(tf.square(_net - labels_node))

#训练器 可以试试不同的训练器，看对训练结果的影响
#optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  
# tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')  
optimizer = tf.train.GradientDescentOptimizer(0.0001).minimize(loss)

def train():
    #with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
    with tf.Session() as sess:
        #初始化参数
        sess.run(tf.global_variables_initializer())

        for j in xrange(0, 100):
            #打印当前网络的输出值
            accurate = 0
            for _i in xrange(0,100):
                testData,testLabel = MNISTData.extract_testdata(onehot = True)
                lbl = sess.run(_test, feed_dict={testOne:testData})

                if np.argmax(lbl, 1) == np.argmax(testLabel, 1):
                    accurate = accurate + 0.01

            
            #执行训练
            totalLoss = 0.0
            for _i in xrange(0,100):
                trainData,trainLabel = MNISTData.extract_traindata(BATCH_SIZE,onehot = True)
                _,_loss = sess.run([optimizer,loss], feed_dict={labels_node: trainLabel, inputlayer: trainData})
                totalLoss = totalLoss + _loss

            print(j,accurate,totalLoss)
            
        #保存已训练的网络
        Saver = []
        for item in plist:
            Saver.append(item.getSaver(sess))
            
        saveToFile = ConvNet.openEmptyFileW("MNIST.txt")
        for item in Saver:
            item(saveToFile)
        saveToFile.flush();saveToFile.close()
        

train()
#test()#可以试着不训练直接跑test，会出来相等的概率