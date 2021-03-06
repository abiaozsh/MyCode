﻿import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time
import os

from six.moves import xrange

BATCH_SIZE = 4
saveSize = [1, 4]

IMAGE_W = 192
IMAGE_H = 256

IMAGE_CHANNEL = 3
LR = 0.0001         # Learning rate


class celebaBetter:
    def __init__(self, filePath):
        self.file_index = 0
        self.content_index = 0
        self.filePath = filePath
        self.bytestream = open(self.filePath + str(self.file_index)+".bin","br")
        self.nextImage = None
        self.t = None
    def extract_data(self):
        def _load_t():
            self.content_index = self.content_index + BATCH_SIZE
            if self.content_index>=4096:#202599
                self.file_index = self.file_index + 1
                if self.file_index >= 31:
                    self.file_index = 0
                self.bytestream.close()
                self.bytestream = open(self.filePath + str(self.file_index)+".bin","br")
                self.content_index = 0
        
            buf = self.bytestream.read(BATCH_SIZE * IMAGE_H * IMAGE_W * IMAGE_CHANNEL)
            data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
            data = (data) / 256.0 - 0.5
            data = data.reshape(BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL)
            self.nextImage = data

        while self.nextImage is None:
            if self.t and self.t.isAlive():
                self.t.join()
                print("glich")
            else:
                self.t = threading.Thread(target=_load_t,args=())
                self.t.start()

        ret = self.nextImage
        self.nextImage = None
    
        self.t = threading.Thread(target=_load_t,args=())
        self.t.start()
    
        return ret

CBHR = celebaBetter("E:\\MNIST\\celebaBetter\\")
CBLR = celebaBetter("E:\\MNIST\\CelebABetterLowRes\\")



print("startload")
glist = []
GF = 64             # Dimension of G filters in first conv layer. default [64]
loadFromFile = ConvNet.openEmptyFileR('gan12srg.txt')

def resblock():
    gcva = ConvNet.addlist(glist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = GF*1,filterSize = 5,biasInitVal=0,loadFromFile = loadFromFile))
    gcvb = ConvNet.addlist(glist,ConvNet.Conv(inDepth = GF*1         ,outDepth = GF*1,filterSize = 5,biasInitVal=0,loadFromFile = loadFromFile))
    gcvc = ConvNet.addlist(glist,ConvNet.Conv(inDepth = GF*1         ,outDepth = GF*1,filterSize = 5,biasInitVal=0,loadFromFile = loadFromFile))
    gcvd = ConvNet.addlist(glist,ConvNet.Conv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,biasInitVal=0,loadFromFile = loadFromFile))
    return gcva,gcvb,gcvc,gcvd

gcva0,gcvb0,gcvc0,gcvd0 = resblock()
gcva1,gcvb1,gcvc1,gcvd1 = resblock()
gcva2,gcvb2,gcvc2,gcvd2 = resblock()
gcva3,gcvb3,gcvc3,gcvd3 = resblock()
gcva4,gcvb4,gcvc4,gcvd4 = resblock()
gcva5,gcvb5,gcvc5,gcvd5 = resblock()

if loadFromFile:loadFromFile.close()

dlist = []
DF = 64             # Dimension of D filters in first conv layer. default [64]
loadFromFile = ConvNet.openEmptyFileR('gan12srd.txt')
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile))#128out
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))#64out
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))#32out
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*2,filterSize = 3,loadFromFile = loadFromFile))#16out
dcv4 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*1,filterSize = 3,loadFromFile = loadFromFile))#8out
dcv5 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = 1,filterSize = 1,loadFromFile = loadFromFile))#8out

if loadFromFile:loadFromFile.close()

print("endload")
def generator(inputT):
    net = inputT
    
    def getLayer(net,gcva,gcvb,gcvc,gcvd):
        inter = gcva.getLayer(net,   convStride = 1, poolSize = 1,isRelu=True, fixed = False)
        inter = gcvb.getLayer(inter, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
        inter = gcvc.getLayer(inter, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
        inter = gcvd.getLayer(inter, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
        return net + inter
    
    net = getLayer(net,gcva0,gcvb0,gcvc0,gcvd0)
    net = getLayer(net,gcva1,gcvb1,gcvc1,gcvd1)
    net = getLayer(net,gcva2,gcvb2,gcvc2,gcvd2)
    net = getLayer(net,gcva3,gcvb3,gcvc3,gcvd3)
    net = getLayer(net,gcva4,gcvb4,gcvc4,gcvd4)
    net = getLayer(net,gcva5,gcvb5,gcvc5,gcvd5)
    
    return net
    
def discriminator(inputT):
    _ret = dcv0.getLayer(inputT, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv2.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv3.getLayer(_ret, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv4.getLayer(_ret, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv5.getLayer(_ret, convStride = 1, poolSize = 1,isRelu=False, fixed = False)
    return _ret


def train():

    imagesHR = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    imagesLR = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    
    G = generator(imagesLR)
    D_logits  = discriminator(imagesHR)
    D_logits_F = discriminator(G)

    gen_cost = -tf.reduce_mean(D_logits_F)
    disc_cost = tf.reduce_mean(D_logits_F) - tf.reduce_mean(D_logits)
    alpha = tf.random_uniform(shape=[BATCH_SIZE,1], minval=0.0,maxval=1.0)
    differences = G - imagesHR
    differences = tf.reshape(differences,[BATCH_SIZE,-1])
    imagereshape = tf.reshape(imagesHR,[BATCH_SIZE,-1])
    interpolates = imagereshape + (alpha*differences)
    interpolates = tf.reshape(interpolates,imagesHR.shape)
    gradients = tf.gradients(discriminator(interpolates), [interpolates])[0]
    slopes = tf.sqrt(tf.reduce_sum(tf.square(gradients), reduction_indices=[1]))
    gradient_penalty = tf.reduce_mean((slopes-1.)**2)
    
    LAMBDA = 10 # Gradient penalty lambda hyperparameter
    disc_cost += LAMBDA*gradient_penalty

    g_vars = ConvNet.getParam(glist)
    d_vars = ConvNet.getParam(dlist)

    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sess = tf.Session()

    init = tf.global_variables_initializer()  
    sess.run(init)
    
    sampleLR = CBLR.extract_data()
    _ = CBHR.extract_data()
    ConvNet.saveImages(sampleLR, saveSize, 'out12sr\\demo.png')
    start_time = time.time()
    idx = 0
    while True:
        idx = idx + 1
        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(CBHR.file_index)+","+str(CBHR.content_index)+","+str(elapsed_time))
        
        for _ in xrange(2):
            loadedimageLR = CBLR.extract_data()
            loadedimageHR = CBHR.extract_data()
            sess.run(d_optim, feed_dict = {imagesLR:loadedimageLR,imagesHR:loadedimageHR})

        loadedimageLR = CBLR.extract_data()
        loadedimageHR = CBHR.extract_data()
        sess.run(g_optim, feed_dict = {imagesLR:loadedimageLR})

        if idx % 200 == 0:

            sample = sess.run(G, feed_dict = {imagesLR:sampleLR})

            def imgSave(idx,sample):
                ConvNet.saveImages(sample, saveSize, 'out12sr\\sample_%d.png' % (idx))
                
                
            t = threading.Thread(target=imgSave,args=(idx,sample))
            t.start()

        exist = False
        
        if idx%10 == 0:
            exist = os.path.exists("stop.txt")
            
        if idx % 2000 == 0 or exist:
            
            def save(idx, gSaver, dSaver):
                print("start save")
                saveToFile = ConvNet.openEmptyFileW("gan12srg"+str(idx)+".txt")
                for item in gSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
 
                saveToFile = ConvNet.openEmptyFileW("gan12srd"+str(idx)+".txt")
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
            
            
        if exist:
            break

    sess.close()
    

train()
