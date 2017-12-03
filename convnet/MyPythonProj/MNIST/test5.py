import tensorflow as tf
import numpy as np

data = np.ndarray([3,2], np.float32)
data[0,0] = 0.01;
data[0,1] = 0.1;
data[1,0] = 0.1;
data[1,1] = 0.1;
data[2,0] = 0.1;
data[2,1] = 0.1;
        
        
        
x = tf.constant(data,tf.float32)
mean, variance = tf.nn.moments(x, [0,1])

bn = tf.nn.batch_normalization(x, 0, variance, 0, 1, 0.0000001)

nm = tf.norm(x)

std = (x-mean) /nm


with tf.Session() as sess:
    m, v = sess.run([mean, variance])
    
    print(data.std())
    
    print(data / data.std())

    print()

    print((data - data.mean()) / data.std())
    
    print(m, v)
    
    print(sess.run(std))
    
    print(sess.run(nm))
    
    print(sess.run(bn))