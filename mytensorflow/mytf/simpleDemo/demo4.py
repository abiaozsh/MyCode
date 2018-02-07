
import tensorflow as tf

#输入（训练数据）
datax = [2,3,4,5,2.3]

#输出（训练数据）
datay = [1,2,3,4,3.4]

w = tf.Variable(0.12)
b = tf.Variable(0.13)

x = tf.placeholder(tf.float32)
y_predict = tf.placeholder(tf.float32)

net = w * x + b
loss = tf.square(net - y_predict)
optimizer = tf.train.GradientDescentOptimizer(0.01).minimize(loss)

with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
    init = tf.global_variables_initializer()
    sess.run(init)
    
    def show():
        for i in range(0,4):
            y = sess.run(net,feed_dict = {x:datax[i]})
            _loss = sess.run(loss, feed_dict = {x:datax[i],y_predict:datay[i]})
            print("input:",datay[i],"output:",y,"loss:",_loss)

        print("")
    def opti():
        for i in range(0,4):
            sess.run(optimizer, feed_dict = {x:datax[i],y_predict:datay[i]})
    
    
    show()
    opti()
    show()
    opti()
    show()
    opti()

    
    
    