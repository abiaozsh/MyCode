import os
import numpy as np
import scipy.misc
import tensorflow as tf

import ConvNet

from six.moves import xrange


BATCH_SIZE = 32
OUTPUT_SIZE = 64
IMAGE_SIZE = 64
GF = 64             # Dimension of G filters in first conv layer. default [64]
DF = 64             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3
LR = 0.0002         # Learning rate
EPOCH = 5
CRITIC_NUM = 5
LOAD_MODEL = False  # Whether or not continue train from saved model。
TRAIN = True
CURRENT_DIR = os.getcwd()

import ConvNet


bytestream = open("e:\\MNIST\\celeba.bin","br")
file_index = 0
def extract_data():
    global file_index
    global bytestream
    file_index = file_index + 64
    if file_index>=202599:#202599
        bytestream.close()
        bytestream = open("e:\\MNIST\\celeba.bin","br")
        file_index = 0

    buf = bytestream.read(BATCH_SIZE * IMAGE_SIZE * IMAGE_SIZE * IMAGE_CHANNEL)
    data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
    data = (data) / 256.0
    data = data.reshape(BATCH_SIZE, IMAGE_SIZE, IMAGE_SIZE, IMAGE_CHANNEL)
    return data



def bias(name, shape, bias_start = 0.0, trainable = True):
    
    var = tf.get_variable(name, shape, tf.float32,trainable = trainable, initializer = tf.constant_initializer(bias_start, dtype = tf.float32))
    return var


def weight(name, shape, stddev = 0.02, trainable = True):
    
    var = tf.get_variable(name, shape, tf.float32,trainable = trainable, initializer = tf.random_normal_initializer(stddev = stddev, dtype = tf.float32))
    return var


def fully_connected(value, output_shape, name = 'fully_connected', with_w = False):
    
    shape = value.get_shape().as_list()
    
    with tf.variable_scope(name):
        biases = bias('biases', [output_shape], 0.0)
        weights = weight('weights', [shape[1], output_shape], 0.02)
        
    if with_w:
        return tf.matmul(value, weights) + biases, weights, biases
    else:
        return tf.matmul(value, weights) + biases


    
def deconv2d(value, output_shape, k_h = 5, k_w = 5, strides =[1, 2, 2, 1], name = 'deconv2d'):
    
    with tf.variable_scope(name):
        biases = bias('biases', [output_shape[-1]])
        weights = weight('weights', [k_h, k_w, output_shape[-1], value.get_shape()[-1]])
        
        deconv = tf.nn.conv2d_transpose(value, weights, output_shape, strides = strides)
        deconv = tf.nn.bias_add(deconv, biases)
        
        return deconv
            
            
def conv2d(value, output_dim, k_h = 5, k_w = 5, strides =[1, 2, 2, 1], name = 'conv2d'):
    
    with tf.variable_scope(name):
        biases = bias('biases', [output_dim])
        weights = weight('weights', [k_h, k_w, value.get_shape()[-1], output_dim])
        
        conv = tf.nn.conv2d(value, weights, strides = strides, padding = 'SAME')
        conv = tf.nn.bias_add(conv, biases)
        
        return conv

def generator(z, name = 'generator'):
    
    with tf.name_scope(name):

        s2, s4, s8, s16 = OUTPUT_SIZE//2, OUTPUT_SIZE//4, OUTPUT_SIZE//8, OUTPUT_SIZE//16
    
        h1 = tf.nn.leaky_relu(tf.reshape(fully_connected(z, GF*8*s16*s16, 'g_fc1'), [-1, s16, s16, GF*8], name = 'reshap'))
        h2 = tf.nn.leaky_relu(deconv2d(h1, [BATCH_SIZE, s8, s8, GF*4], name = 'g_deconv2d1'))
        h3 = tf.nn.leaky_relu(deconv2d(h2, [BATCH_SIZE, s4, s4, GF*2], name = 'g_deconv2d2'))
        h4 = tf.nn.leaky_relu(deconv2d(h3, [BATCH_SIZE, s2, s2, GF*1], name = 'g_deconv2d3'))
        h5 = deconv2d(h4, [BATCH_SIZE, OUTPUT_SIZE, OUTPUT_SIZE, 3], name = 'g_deconv2d4')    
        
        return h5
    
    
def discriminator(image, reuse = False, name = 'discriminator'):
    
    with tf.name_scope(name):    
    
        if reuse:
            tf.get_variable_scope().reuse_variables()
        
        h0 = tf.nn.leaky_relu(conv2d(image, DF, name='d_h0_conv'), name = 'd_h0_lrelu')
        h1 = tf.nn.leaky_relu(conv2d(h0, DF*2, name='d_h1_conv'), name = 'd_h1_lrelu')
        h2 = tf.nn.leaky_relu(conv2d(h1, DF*4, name='d_h2_conv'), name = 'd_h2_lrelu')
        h3 = tf.nn.leaky_relu(conv2d(h2, DF*8, name='d_h3_conv'), name = 'd_h3_lrelu')
        h4 = fully_connected(tf.reshape(h3, [BATCH_SIZE, -1]), 1, 'd_h4_fc')#用fclayer转成0,1
        
        return h4
    
        
def sampler(z, name = 'sampler'):
    with tf.name_scope(name):
        tf.get_variable_scope().reuse_variables()
        return generator(z)
    

def save_images(images, size, path):
    img = (images + 1.0) / 2.0
    h, w = img.shape[1], img.shape[2]
    merge_img = np.zeros((h * size[0], w * size[1], 3))
    for idx, image in enumerate(images):
        i = idx % size[1]
        j = idx // size[1]
        merge_img[j*h:j*h+h, i*w:i*w+w, :] = image
        
    return scipy.misc.imsave(path, merge_img)    

def saveimg(filename,wholeData,idx,scale = 255,bias = 128):
    data = ConvNet.newImage(wholeData.shape[1],wholeData.shape[2])
    #clearImg(data,0)
    for i in xrange(0,wholeData.shape[1]):
        for j in xrange(0,wholeData.shape[2]):
            r = wholeData[idx,j,i,0] * scale + bias
            g = wholeData[idx,j,i,1] * scale + bias
            b = wholeData[idx,j,i,2] * scale + bias
            ConvNet.setpixel(data,i,j,r,g,b)
    #lbl = wholeData[idx]
    ConvNet.saveImg(data, filename)

def train():
    ###################
#     loadedimage = extract_data()
#     save_images(loadedimage, [8, 8],'data0.png')
#     saveimg("test1.png",loadedimage,0,scale = 255,bias = 0)
#  
#     loadedimage = extract_data()
#     save_images(loadedimage, [8, 8],'data1.png')
#      
#     exit()
    ###################

    global_step = tf.Variable(0, name = 'global_step', trainable = False)

    train_dir = CURRENT_DIR + '/logs_without_condition/'

    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_SIZE, IMAGE_SIZE, IMAGE_CHANNEL])

    z = tf.placeholder(tf.float32, [BATCH_SIZE, Z_DIM])

    G = generator(z)
    D_logits  = discriminator(images)
    samples = sampler(z)
    D_logits_F = discriminator(G, reuse = True)
    
    
    gen_cost = -tf.reduce_mean(D_logits_F)
    disc_cost = tf.reduce_mean(D_logits_F) - tf.reduce_mean(D_logits)
    alpha = tf.random_uniform(shape=[BATCH_SIZE,1], minval=0.0,maxval=1.0)
    differences = G - images
    differences = tf.reshape(differences,[BATCH_SIZE,-1])
    imagereshape = tf.reshape(images,[BATCH_SIZE,-1])
    interpolates = imagereshape + (alpha*differences)
    interpolates = tf.reshape(interpolates,images.shape)
    gradients = tf.gradients(discriminator(interpolates), [interpolates])[0]
    slopes = tf.sqrt(tf.reduce_sum(tf.square(gradients), reduction_indices=[1]))
    gradient_penalty = tf.reduce_mean((slopes-1.)**2)
    
    LAMBDA = 10 # Gradient penalty lambda hyperparameter
    disc_cost += LAMBDA*gradient_penalty

    t_vars = tf.trainable_variables()
    d_vars = [var for var in t_vars if 'd_' in var.name]
    g_vars = [var for var in t_vars if 'g_' in var.name]

    saver = tf.train.Saver()
    
    with tf.variable_scope("", reuse=tf.AUTO_REUSE):
        d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
        g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)


    sess = tf.Session()
    
    
    sample_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    
    init = tf.initialize_all_variables()  
    sess.run(init)





    start = 0
    if LOAD_MODEL:        
        print(" [*] Reading checkpoints...")
        ckpt = tf.train.get_checkpoint_state(train_dir)        

        if ckpt and ckpt.model_checkpoint_path:
            ckpt_name = os.path.basename(ckpt.model_checkpoint_path)
            saver.restore(sess, os.path.join(train_dir, ckpt_name))
            global_step = ckpt.model_checkpoint_path.split('/')[-1].split('-')[-1]
            print('Loading success, global_step is %s' % global_step)
            
        start = int(global_step)
        
    for epoch in xrange(EPOCH):
        
        batch_idxs = 3072
        
        if epoch:
            start = 0
            
        for idx in xrange(start, batch_idxs):
            #if idx<25 or idx % 500 == 0:
            #    critic_num = 25
            #else:
            critic_num = 5
            print(idx)
            for _ in xrange(critic_num):
                batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
                loadedimage = extract_data()
                sess.run(d_optim, feed_dict = {z:batch_z, images:loadedimage})

            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            sess.run(g_optim, feed_dict = {z: batch_z})

            if idx % 100 == 0:
                sample = sess.run(samples, feed_dict = {z: sample_z})
                samples_path = CURRENT_DIR + '\\out\\'
                save_images(sample, [8, 8], 
                            samples_path + \
                            'sample_%d_epoch_%d.png' % (epoch, idx))

                print('\n'*2)
                print('===========    %d_epoch_%d.png save down    ===========' 
                                                                %(epoch, idx))
                print('\n'*2)
                
            if (idx % 512 == 0) or (idx + 1 == batch_idxs):
                checkpoint_path = os.path.join(train_dir, 
                                               'my_dcgan_tfrecords.ckpt')
                saver.save(sess, checkpoint_path, global_step = idx+1)
                print('*********    model saved    *********')

        print('******* start with %d *******' % start)
    
    sess.close()
    
    
    
def evaluate():
    eval_dir = CURRENT_DIR + "\\out\\"
     
    checkpoint_dir = CURRENT_DIR + '/logs_without_condition/'
     
    z = tf.placeholder(tf.float32, [None, Z_DIM], name='z')
     
    G = generator(z)
     
    sample_z1 = np.random.uniform(-1, 1, size=(BATCH_SIZE, Z_DIM))
    sample_z2 = np.random.uniform(-1, 1, size=(BATCH_SIZE, Z_DIM))
    sample_z3 = (sample_z1 + sample_z2) / 2
    sample_z4 = (sample_z1 + sample_z3) / 2
    sample_z5 = (sample_z2 + sample_z3) / 2    
     
    print("Reading checkpoints...")
    ckpt = tf.train.get_checkpoint_state(checkpoint_dir)
     
    saver = tf.train.Saver(tf.all_variables())
     
    sess = tf.Session()
     
    if ckpt and ckpt.model_checkpoint_path:
        ckpt_name = os.path.basename(ckpt.model_checkpoint_path)
        global_step = ckpt.model_checkpoint_path.split('/')[-1].split('-')[-1]        
        saver.restore(sess, os.path.join(checkpoint_dir, ckpt_name))
        print('Loading success, global_step is %s' % global_step)
     
    eval_sess1 = sess.run(G, feed_dict = {z: sample_z1})
    eval_sess2 = sess.run(G, feed_dict = {z: sample_z4})
    eval_sess3 = sess.run(G, feed_dict = {z: sample_z3})
    eval_sess4 = sess.run(G, feed_dict = {z: sample_z5})
    eval_sess5 = sess.run(G, feed_dict = {z: sample_z2})
     
    print(eval_sess3.shape)
     
    save_images(eval_sess1, [8, 8], eval_dir + 'eval_%d.png' % 1)
    save_images(eval_sess2, [8, 8], eval_dir + 'eval_%d.png' % 2)
    save_images(eval_sess3, [8, 8], eval_dir + 'eval_%d.png' % 3)
    save_images(eval_sess4, [8, 8], eval_dir + 'eval_%d.png' % 4)
    save_images(eval_sess5, [8, 8], eval_dir + 'eval_%d.png' % 5)
     
     
    sess.close()


train()
#evaluate()