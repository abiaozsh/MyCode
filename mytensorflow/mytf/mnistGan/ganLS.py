import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time

from six.moves import xrange

import MNISTDataLargeSet

BATCH_SIZE = 200
testBATCH_SIZE = 40
saveSize = [4, 10]

GF = 64             # Dimension of G filters in first conv layer. default [64]
DF = 64             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 64

LR = 0.0001         # Learning rate

t2, t4 = MNISTDataLargeSet.IMAGE_H//2, MNISTDataLargeSet.IMAGE_H//4
s2, s4 = MNISTDataLargeSet.IMAGE_W//2, MNISTDataLargeSet.IMAGE_W//4

glist = []
loadFromFile = ConvNet.openEmptyFileR('gan0g.txt')
gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM,outDepth = Z_DIM,loadFromFile = loadFromFile))
gfc1 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM,outDepth = GF*2*t4*s4,loadFromFile = loadFromFile))
gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFile = loadFromFile))
gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*1,outDepth = MNISTDataLargeSet.IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

dlist = []
loadFromFile = ConvNet.openEmptyFileR('gan0d.txt')
dcva = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = MNISTDataLargeSet.IMAGE_CHANNEL,outDepth = DF*1,filterSize = 1,loadFromFile = loadFromFile))
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile))
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*2*t4*s4,outDepth = 1,loadFromFile = loadFromFile))
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*2*t4*s4,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

def generator(z):
    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
    _ret = gfc1.getLayer(_ret, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, t4, s4, GF*2)
    _ret = gdc2.getLayer(_ret, height = t2, width = s2, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = MNISTDataLargeSet.IMAGE_H, width = MNISTDataLargeSet.IMAGE_W, convStride = 2, isRelu=False, fixed = False)
    return _ret
    
def discriminator(inputT):
    _ret = dcva.getLayer(inputT, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv0.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=False, fixed = False)
    return _ret

def train():

    images = tf.placeholder(tf.float32, [BATCH_SIZE, MNISTDataLargeSet.IMAGE_H, MNISTDataLargeSet.IMAGE_W, MNISTDataLargeSet.IMAGE_CHANNEL])

    z = tf.placeholder(tf.float32, [BATCH_SIZE, Z_DIM])

    testz = tf.placeholder(tf.float32, [testBATCH_SIZE, Z_DIM])

    G = generator(z)
    D_logits  = discriminator(images)
    samples = generator(testz)
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

    g_vars = ConvNet.getParam(glist)
    d_vars = ConvNet.getParam(dlist)

    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sample_z = np.random.uniform(-1, 1, size = (testBATCH_SIZE, Z_DIM))


    sess = tf.Session()
    
    init = tf.global_variables_initializer()  
    sess.run(init)

    start_time = time.time()
    for idx in xrange(0, 50001):
        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(elapsed_time))
        
        for _ in xrange(2):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage,_ = MNISTDataLargeSet.extract_data(BATCH_SIZE)

            sess.run(d_optim, feed_dict = {z:batch_z, images:loadedimage})

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z})

        if idx % 50 == 0:

            sample = sess.run(samples, feed_dict = {testz: sample_z})

            def imgSave(idx,sample):
                ConvNet.saveImagesMono(sample, saveSize, 'out\\sample_%d.png' % (idx))
                
                
            t = threading.Thread(target=imgSave,args=(idx,sample))
            t.start()
            
        if idx % 500 == 0:
            
            def save(idx, gSaver, dSaver):
                print("start save")
                saveToFile = ConvNet.openEmptyFileW("gan0g"+str(idx)+".txt")
                for item in gSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
 
                saveToFile = ConvNet.openEmptyFileW("gan0d"+str(idx)+".txt")
                for item in dSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
                print("end save")
            
            gSaver = []
            dSaver = []
            for item in glist:
                gSaver.append(item.getSaver(sess))
            for item in dlist:
                dSaver.append(item.getSaver(sess))

            t = threading.Thread(target=save,args=(idx,gSaver, dSaver))
            t.start()
            
            
            

    sess.close()
    

train()
