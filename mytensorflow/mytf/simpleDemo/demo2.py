﻿
import tensorflow as tf

# y = f(x)

#输入（训练数据）
datax = [2,3,4,5]

#输出（训练数据）
datay = [1,2,3,4]

w = tf.Variable(0.12)
b = tf.Variable(0.13)

x = tf.placeholder(tf.float32)
y_predict = tf.placeholder(tf.float32)

net = w * x + b
loss = tf.square(net - y_predict)

with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
    init = tf.global_variables_initializer()
    sess.run(init)
    
    for i in range(0,4):
        y = sess.run(net,feed_dict = {x:datax[i]})
        
        _loss = sess.run(loss, feed_dict = {x:datax[i],y_predict:datay[i]})
        print("input:"+str(datay[i])+"\t output:"+str(y) + "\t loss:"+str(_loss))
