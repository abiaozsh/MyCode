import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time
import os

from six.moves import xrange

BATCH_SIZE = 256

IMAGE_W = 192
IMAGE_H = 256

Z_DIM = 128
IMAGE_CHANNEL = 3
LR = 0.002         # Learning rate

class celebaBetter:
    def __init__(self, filePath):
        self.file_index = 0
        self.content_index = 0
        self.filePath = filePath
        self.bytestream = open(self.filePath + str(self.file_index)+".bin","br")
        self.nextImage = None
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

CBHR = celebaBetter("F:\\MNIST\\celebaBetter\\")


print("startload")
glist = []
GF = 96             # Dimension of G filters in first conv layer. default [64]
loadFromFile = ConvNet.openEmptyFileR('gan12g.txt')
gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*3*4,loadFromFile = loadFromFile))
gdc0 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 3,loadFromFile = loadFromFile))#4in
gdc1 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 3,loadFromFile = loadFromFile))#8in
gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFile = loadFromFile))#16in
gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile))#32in 64out
if loadFromFile:loadFromFile.close()

dlist = []
DF = 96             # Dimension of D filters in first conv layer. default [64]
loadFromFile = ConvNet.openEmptyFileR('gan12d.txt')
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile))#32out
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))#16out
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 3,loadFromFile = loadFromFile))#8out
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 3,loadFromFile = loadFromFile))#4out
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*8*3*4,outDepth = 128,loadFromFile = loadFromFile))
dfc1 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = 128,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()
print("endload")
def generator(z):
    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, 4, 3, GF*8)
    _ret = gdc0.getLayer(_ret, height = 8, width = 6, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc1.getLayer(_ret, height = 16, width = 12, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc2.getLayer(_ret, height = 32, width = 24, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = 64, width = 48, convStride = 2, isRelu=False, fixed = False)
    return _ret
    
def discriminator(inputT):
    _ret = dcv0.getLayer(inputT, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv2.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv3.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=True, fixed = False)
    _ret = dfc1.getLayer(_ret, isRelu=False, fixed = False)
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
    z = tf.placeholder(tf.float32, [BATCH_SIZE, Z_DIM])

    smallImages = tf.nn.avg_pool(images, ksize=[1, 4, 4, 1], strides=[1, 4, 4, 1], padding="SAME")
    G = generator(z)
    D_logits  = discriminator(smallImages)
    D_logits_F = discriminator(G)

    gen_cost = -tf.reduce_mean(D_logits_F)
    disc_cost = tf.reduce_mean(D_logits_F) - tf.reduce_mean(D_logits)
    alpha = tf.random_uniform(shape=[BATCH_SIZE,1], minval=0.0,maxval=1.0)
    differences = G - smallImages
    differences = tf.reshape(differences,[BATCH_SIZE,-1])
    imagereshape = tf.reshape(smallImages,[BATCH_SIZE,-1])
    interpolates = imagereshape + (alpha*differences)
    interpolates = tf.reshape(interpolates,smallImages.shape)
    gradients = tf.gradients(discriminator(interpolates), [interpolates])[0]
    slopes = tf.sqrt(tf.reduce_sum(tf.square(gradients), reduction_indices=[1]))
    gradient_penalty = tf.reduce_mean((slopes-1.0)**2)
    
    LAMBDA = 10 # Gradient penalty lambda hyperparameter
    disc_cost += LAMBDA*gradient_penalty

    g_vars = ConvNet.getParam(glist)
    d_vars = ConvNet.getParam(dlist)

    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sess = tf.Session()

    sample_z1 = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    #for i in xrange(0,BATCH_SIZE):
    #    sample_z[i] = np.random.uniform(-(i/BATCH_SIZE), (i/BATCH_SIZE), size = (Z_DIM))
    
    init = tf.global_variables_initializer()  
    sess.run(init)

    start_time = time.time()
    idx = 0
    while True:
        idx = idx + 1
        global file_index
        global content_index
        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(file_index)+","+str(content_index)+","+str(elapsed_time))
        
        for _ in xrange(2):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage = CBHR.extract_data()
            sess.run(d_optim, feed_dict = {z:batch_z, images:loadedimage})

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z})

        if idx % 200 == 0:

            sample = np.zeros([BATCH_SIZE*1, IMAGE_H//4, IMAGE_W//4, IMAGE_CHANNEL], dtype=np.float32)
            sample[BATCH_SIZE*0:BATCH_SIZE*1] = sess.run(G, feed_dict = {z: sample_z1})

            def imgSave(idx,sample):
                ConvNet.saveImages(sample, [8,32], 'out12\\sample_%d.png' % (idx))
                
                
            t = threading.Thread(target=imgSave,args=(idx,sample))
            t.start()
        
        exist = False
        
        if idx%10 == 0:
            exist = os.path.exists("stop.txt")
            
        if idx % 2000 == 0 or exist:
            
            def save(idx, gSaver, dSaver):
                print("start save")
                saveToFile = ConvNet.openEmptyFileW("gan12g"+str(idx)+".txt")
                for item in gSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
 
                saveToFile = ConvNet.openEmptyFileW("gan12d"+str(idx)+".txt")
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
