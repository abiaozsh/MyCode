import tensorflow as tf
import numpy as np
import ConvNet #我写的tensorflow的壳，不用太在意，主要是打包层，和保存层的参数，tensorflow有自己的序列化方法，不过和我以前用的框架不通用所以我自己写了一个

batchSize = 3

#训练数据： 输入网络这3条数据，不断训练
#期望网络在训练后， 输入某一组数据，就能返回同样的数据（回归任务，相对的是分类任务）
#也可以把输入和输出分成两组，让这两组形成对应关系
indata = np.mat([
    [1.0, 3.0, 1.0, 3.0],
    [2.0, 1.0, 2.0, 1.0],
    [0.0, 0.5, 1.0, 1.5],
    ])

#用来验证的数据，和训练数据一样，也可以用不同的数据验证
verifydata1 = np.ndarray([batchSize, 4], np.float32)
verifydata1[0] = [1.0, 3.0, 1.0, 3.0]
verifydata2 = np.ndarray([batchSize, 4], np.float32)
verifydata2[0] = [2.0, 1.0, 2.0, 1.0]
verifydata3 = np.ndarray([batchSize, 4], np.float32)
verifydata3[0] = [0.0, 0.5, 1.0, 1.5]

#输入层
inputlayer = tf.placeholder(tf.float32, shape=(batchSize, 4))

#预期的数据
finaldata = tf.placeholder(tf.float32, shape=(batchSize, 4))

#网络定义
plist = []
loadFromFile = ConvNet.openEmptyFileR('test.txt')#从文件中加载已训练的，文件不存在的话默认会随机初始化网络
#可以调整 5 这个数字，看对最终结果的影响，数字太小网络容量不足无法学习太复杂的情况
fc0 = ConvNet.addlist(plist,ConvNet.FC(inDepth = 4,outDepth = 5,loadFromFile = loadFromFile))
fc1 = ConvNet.addlist(plist,ConvNet.FC(inDepth = 5,outDepth = 4,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()   

def net(inputT):
    _ret = fc0.getLayer(inputT, isRelu=True, fixed = False)
    _ret = fc1.getLayer(_ret, isRelu=False, fixed = False)
    return _ret

_net = net(inputlayer)

#损失函数
# loss = tf.reduce_mean(tf.nn.sparse_softmax_cross_entropy_with_logits(labels=train_labels_node, logits=logits))  #这个loss是用来处理分类的情况，现在是回归所以不用
loss = tf.reduce_sum(tf.square(_net - finaldata))

#训练器
#optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')  #这个 训练器 是用来处理分类的情况，现在是回归所以不用
optimizer = tf.train.GradientDescentOptimizer(0.001).minimize(loss)

def train():
    with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
        #初始化参数
        sess.run(tf.global_variables_initializer())
    
        for j in range(0, 20):
            #打印当前网络的输出值
            a1 = sess.run(_net, feed_dict={inputlayer:verifydata1})
            a2 = sess.run(_net, feed_dict={inputlayer:verifydata2})
            a3 = sess.run(_net, feed_dict={inputlayer:verifydata3})
            print(j, a1[0], a2[0], a3[0])#可以看到输出值越来越接近输入的值
            
            #执行训练
            for i in range(0, 100):
                sess.run(optimizer, feed_dict={finaldata: indata, inputlayer: indata})
        
        #保存已训练的网络
        Saver = []
        for item in plist:
            Saver.append(item.getSaver(sess))
            
        saveToFile = ConvNet.openEmptyFileW("test.txt")
        for item in Saver:
            item(saveToFile)
        saveToFile.flush();saveToFile.close()

        
def test():
    with tf.Session() as sess:
        sess.run(tf.global_variables_initializer())
        a1 = sess.run(_net, feed_dict={inputlayer:verifydata1})
        a2 = sess.run(_net, feed_dict={inputlayer:verifydata2})
        a3 = sess.run(_net, feed_dict={inputlayer:verifydata3})
        print(a1[0], a2[0], a3[0])
        
train()
#test()#可以试着不训练直接跑test，会出来乱七八糟的数字