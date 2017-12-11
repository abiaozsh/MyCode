import numpy as np

import tensorflow as tf
import threading
import ConvNet

from six.moves import xrange

BATCH_SIZE = 16

IMAGE_W = 160
IMAGE_H = 208

GF = 64             # Dimension of G filters in first conv layer. default [64]
DF = 64             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3
LR = 0.0002         # Learning rate
EPOCH = 5
CRITIC_NUM = 5
TRAIN = True

file_index = 0
content_index = 0
bytestream = open("e:\\MNIST\\celebaHI\\"+str(file_index)+".bin","br")
def extract_data():
    global file_index
    global content_index
    global bytestream

    content_index = content_index + BATCH_SIZE
    if content_index>=4096:#202599
        file_index = file_index + 1
        if file_index >= 49:
            file_index = 0
        bytestream.close()
        bytestream = open("e:\\MNIST\\celebaHI\\"+str(file_index)+".bin","br")
        content_index = 0

    buf = bytestream.read(BATCH_SIZE * IMAGE_H * IMAGE_W * IMAGE_CHANNEL)
    data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
    data = (data) / 256.0 - 0.5
    data = data.reshape(BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL)
    return data

t2, t4, t8, t16 = IMAGE_H//2, IMAGE_H//4, IMAGE_H//8, IMAGE_H//16
s2, s4, s8, s16 = IMAGE_W//2, IMAGE_W//4, IMAGE_W//8, IMAGE_W//16

loadFromFile = ConvNet.openEmptyFileR('gan9g.txt')
gfc0 = ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*t16*s16,loadFromFile = loadFromFile)
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
dfc0 = ConvNet.FC(inDepth = DF*8*t16*s16,outDepth = 1,loadFromFile = loadFromFile)
if loadFromFile:loadFromFile.close()

def generator(z):
    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, t16, s16, GF*8)
    _ret = gdc0.getLayer(_ret, height = t8, width = s8, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc1.getLayer(_ret, height = t4, width = s4, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc2.getLayer(_ret, height = t2, width = s2, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = IMAGE_H, width = IMAGE_W, convStride = 2, isRelu=False, fixed = False)
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


def train():
    ###################
#     loadedimage = extract_data()
#     ConvNet.saveImages(loadedimage, [8, 8], "test0.png")
#     loadedimage = extract_data()
#     ConvNet.saveImages(loadedimage, [8, 8], "test1.png")
#     exit()

    
    
#     for idx in xrange(0, 1000000000):
#         loadedimage = extract_data()
#         global file_index
#         global content_index
#         print(str(file_index)+","+str(content_index))         
#     exit()

    ###################
    
    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
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

    g_vars = ConvNet.getParam([gfc0,gdc0,gdc1,gdc2,gdc3])
    d_vars = ConvNet.getParam([dcv0,dcv1,dcv2,dcv3,dfc0])

    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sess = tf.Session()

    sample_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    
    init = tf.initialize_all_variables()  
    sess.run(init)

    for idx in xrange(0, 10000):
        global file_index
        global content_index
        print(str(idx)+","+str(file_index)+","+str(content_index))
        
        for _ in xrange(5):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage = extract_data()
            sess.run(d_optim, feed_dict = {z:batch_z, images:loadedimage})

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z})

        if idx % 50 == 0:
            sample = sess.run(samples, feed_dict = {z: sample_z})
            ConvNet.saveImages(sample, [8, 8], 'out\\sample_%d.png' % (idx))
            
        if idx % 100 == 0:
            
            def save(
                bgfc0, wgfc0,
                bgdc0, wgdc0,
                bgdc1, wgdc1,
                bgdc2, wgdc2,
                bgdc3, wgdc3,
                bdcv0, wdcv0,
                bdcv1, wdcv1,
                bdcv2, wdcv2,
                bdcv3, wdcv3,
                bdfc0, wdfc0
                    ):
                print("start save")
                saveToFile = ConvNet.openEmptyFileW('gan9g.txt')
                gfc0.save_ToFile(bgfc0, wgfc0, saveToFile)
                gdc0.save_ToFile(bgdc0, wgdc0, saveToFile)
                gdc1.save_ToFile(bgdc1, wgdc1, saveToFile)
                gdc2.save_ToFile(bgdc2, wgdc2, saveToFile)
                gdc3.save_ToFile(bgdc3, wgdc3, saveToFile)
                saveToFile.flush();saveToFile.close()

                saveToFile = ConvNet.openEmptyFileW('gan9d.txt')
                dcv0.save_ToFile(bdcv0, wdcv0, saveToFile)
                dcv1.save_ToFile(bdcv1, wdcv1, saveToFile)
                dcv2.save_ToFile(bdcv2, wdcv2, saveToFile)
                dcv3.save_ToFile(bdcv3, wdcv3, saveToFile)
                dfc0.save_ToFile(bdfc0, wdfc0, saveToFile)
                saveToFile.flush();saveToFile.close()
                print("end save")
                
            bgfc0, wgfc0 = gfc0.save_getParam(sess)
            bgdc0, wgdc0 = gdc0.save_getParam(sess)
            bgdc1, wgdc1 = gdc1.save_getParam(sess)
            bgdc2, wgdc2 = gdc2.save_getParam(sess)
            bgdc3, wgdc3 = gdc3.save_getParam(sess)
            bdcv0, wdcv0 = dcv0.save_getParam(sess)
            bdcv1, wdcv1 = dcv1.save_getParam(sess)
            bdcv2, wdcv2 = dcv2.save_getParam(sess)
            bdcv3, wdcv3 = dcv3.save_getParam(sess)
            bdfc0, wdfc0 = dfc0.save_getParam(sess)

            t =threading.Thread(target=save,args=(
                bgfc0, wgfc0,
                bgdc0, wgdc0,
                bgdc1, wgdc1,
                bgdc2, wgdc2,
                bgdc3, wgdc3,
                bdcv0, wdcv0,
                bdcv1, wdcv1,
                bdcv2, wdcv2,
                bdcv3, wdcv3,
                bdfc0, wdfc0
                    ))
            t.start()
            
            
            

    sess.close()
    

train()
