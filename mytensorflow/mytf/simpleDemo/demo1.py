
import tensorflow as tf

datax = [2,3,4,5]
datay = [1,2,3,4]

#w = tf.Variable(0.12,dtype=tf.float32)
#b = tf.Variable(0.13,dtype=tf.float32)

w = tf.Variable(1,dtype=tf.float32)
b = tf.Variable(-1,dtype=tf.float32)


x = tf.placeholder(tf.float32)

net = w*x+b

with tf.Session(config=tf.ConfigProto(device_count = {'GPU': 0})) as sess:
    init = tf.global_variables_initializer()
    sess.run(init)
    
    print(sess.run(w))
    print(sess.run(b))
    
    print()
    

    for i in range(0,4):
        y = sess.run(net,feed_dict = {x:datax[i]})
        print(y)
