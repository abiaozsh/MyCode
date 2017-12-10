import os
import numpy as np
import scipy.misc
import tensorflow as tf

import ConvNet

from six.moves import xrange

BATCH_SIZE = 64

OUTPUT_SIZE = 64
IMAGE_SIZE = 64

GF = 64             # Dimension of G filters in first conv layer. default [64]
DF = 64             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3
LR = 0.0002         # Learning rate
EPOCH = 5
CRITIC_NUM = 5
TRAIN = True
CURRENT_DIR = os.getcwd()

bytestream = open("e:\\MNIST\\celeba.bin","br")
file_index = 0
def extract_data():
    global file_index
    global bytestream
    file_index = file_index + 64
    if file_index>=64*10:#202599
        bytestream.close()
        bytestream = open("e:\\MNIST\\celeba.bin","br")
        file_index = 0

    buf = bytestream.read(BATCH_SIZE * IMAGE_SIZE * IMAGE_SIZE * IMAGE_CHANNEL)
    data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
    data = (data) / 256.0 - 0.5
    data = data.reshape(BATCH_SIZE, IMAGE_SIZE, IMAGE_SIZE, IMAGE_CHANNEL)
    return data

s2, s4, s8, s16 = OUTPUT_SIZE//2, OUTPUT_SIZE//4, OUTPUT_SIZE//8, OUTPUT_SIZE//16


loadFromFile = ConvNet.openEmptyFileR('gan9g.txt')
gfc0 = ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*s16*s16,loadFromFile = loadFromFile)
gdc0 = ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 5,loadFromFile = loadFromFile)
gdc1 = ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile)
gdc2 = ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFile = loadFromFile)
gdc3 = ConvNet.DeConv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile)
if loadFromFile:loadFromFile.close()

loadFromFile = ConvNet.openEmptyFileR('gan9d.txt')
dcv0 = ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile)
dcv1 = ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile)
dcv2 = ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFile = loadFromFile)
dcv3 = ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 5,loadFromFile = loadFromFile)
dfc0 = ConvNet.FC(inDepth = DF*8*s16*s16,outDepth = 1,loadFromFile = loadFromFile)
if loadFromFile:loadFromFile.close()

def generator(z):
    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, s16, s16, GF*8)
    _ret = gdc0.getLayer(_ret, height = s8, width = s8, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc1.getLayer(_ret, height = s4, width = s4, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc2.getLayer(_ret, height = s2, width = s2, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = OUTPUT_SIZE, width = OUTPUT_SIZE, convStride = 2, isRelu=False, fixed = False)
    return _ret
    
def discriminator(inputT):
    _ret = dcv0.getLayer(inputT, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv2.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv3.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=False, fixed = False)
    return _ret

def sampler(z):
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

#转置
def saveimg(filename,wholeData,idx,scale = 255,bias = 128):
    data = ConvNet.newImage(wholeData.shape[1],wholeData.shape[2])
    #clearImg(data,0)
    for i in xrange(0,wholeData.shape[1]):
        for j in xrange(0,wholeData.shape[2]):
            r = wholeData[idx,i,j,0] * scale + bias
            g = wholeData[idx,i,j,1] * scale + bias
            b = wholeData[idx,i,j,2] * scale + bias
            ConvNet.setpixel(data,i,j,r,g,b)
    #lbl = wholeData[idx]
    ConvNet.saveImg(data, filename)

def train():
    ###################
#     loadedimage = extract_data()
#     saveimg("data00.png",loadedimage,0)
#     save_images(loadedimage, [8, 8],'data0.png')
#     loadedimage = extract_data()
#     save_images(loadedimage, [8, 8],'data1.png')
#     exit()
    ###################

    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_SIZE, IMAGE_SIZE, IMAGE_CHANNEL])
    z = tf.placeholder(tf.float32, [BATCH_SIZE, Z_DIM], name='z')

    G = generator(z)
    D_logits  = discriminator(images)
    samples = sampler(z)
    D_logits_F = discriminator(G)

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

    #t_vars = tf.trainable_variables()
    #d_vars = [var for var in t_vars if 'd_' in var.name]
    #g_vars = [var for var in t_vars if 'g_' in var.name]
    g_vars = ConvNet.getParam([gfc0,gdc0,gdc1,gdc2,gdc3])
    d_vars = ConvNet.getParam([dcv0,dcv1,dcv2,dcv3,dfc0])

    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sess = tf.Session()

    sample_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    
    init = tf.initialize_all_variables()  
    sess.run(init)

    for idx in xrange(0, 10000):
        print(idx)
        for _ in xrange(5):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage = extract_data()
            sess.run(d_optim, feed_dict = {z:batch_z, images:loadedimage})

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z})

        if idx % 10 == 0:
            sample = sess.run(samples, feed_dict = {z: sample_z})
            samples_path = CURRENT_DIR + '\\out\\'
            save_images(sample, [8, 8], samples_path + 'sample_%d.png' % (idx))


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