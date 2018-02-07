
import tensorflow as tf

# y = f(x)

#输入（训练数据）（这里是简单的数据，真实的case可能是身高，或者身高+体重）
datax = [2,3,4,5]

#输出（训练数据）（这里是简单的数据，真实的case可能是性别，体脂指数，等等）
datay = [1,2,3,4]

w = tf.Variable(0.12,dtype=tf.float32)#随意的值
b = tf.Variable(0.13,dtype=tf.float32)#随意的值

#w = tf.Variable(1,dtype=tf.float32)
#b = tf.Variable(-1,dtype=tf.float32)

x = tf.placeholder(tf.float32)

net = w*x+b

with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
    init = tf.global_variables_initializer()
    sess.run(init)
    
    print("w is :" + str(sess.run(w)))
    print("b is :" + str(sess.run(b)))
    
    print()
    

    for i in range(0,4):
        y = sess.run(net,feed_dict = {x:datax[i]})
        print("input:"+str(datax[i])+"\t output:"+str(y))
