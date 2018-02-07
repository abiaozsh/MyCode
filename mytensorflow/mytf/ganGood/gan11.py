import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time
import os
import random
import scipy.misc
from six.moves import xrange

BATCH_SIZE = 10

IMAGE_W = 192
IMAGE_H = 256

Z_DIM = 128
IMAGE_CHANNEL = 3



def read_image(path):
    image = scipy.misc.imread(path)
    image = np.delete(image,3,2)
    
    if random.uniform(-1,1)>0 :
        image = np.flip(image, 1)
    
    image = image/255.0 - 0.5
    
    return image

def getFullPath(_sname):
    i = int(_sname[0:6])
    folder1 = str(i // 10000);
    folder2 = str(i // 1000);
    folder3 = str(i // 100);
    dirs = "E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\"+folder1+"\\"+folder2+"\\"+folder3+"\\"+_sname;
    return dirs

class celebaBetter:
    def __init__(self, _dir):
        self._dir = _dir
        self._file = os.listdir(_dir)
        self.nextImage = None
        self.t = None
    def _load_t(self):
        buff = np.ndarray([BATCH_SIZE, IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)
        for i in xrange(0,BATCH_SIZE):
            idx = int(random.uniform(0, len(self._file)))
            
            _name = self._file[idx]
            _name = _name[len(_name)-10:len(_name)]
            _name = getFullPath(_name)
            data = read_image(_name)
            buff[i] = data

        #return buff
        self.nextImage = buff
        self.t = None

    def extract_data(self):

        while self.nextImage is None:
            if self.t and self.t.isAlive():
                self.t.join()
                print("glich")
            else:
                self.t = threading.Thread(target=self._load_t,args=())
                self.t.start()

        ret = self.nextImage
        self.nextImage = None
    
        self.t = threading.Thread(target=self._load_t,args=())
        self.t.start()
    
        return ret

CBHR = celebaBetter("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\gangood")

print("startload")
glist = []
GF = 96             # Dimension of G filters in first conv layer. default [64]
loadFromFileBin = ConvNet.openBinaryFileR('gan11g.bin')
gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*3*4,loadFromFileBin = loadFromFileBin))
gdc0 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*8,filterSize = 3,loadFromFileBin = loadFromFileBin))#4in
gdc1 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 3,loadFromFileBin = loadFromFileBin))#8in
gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 5,loadFromFileBin = loadFromFileBin))#16in
gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*2,filterSize = 5,loadFromFileBin = loadFromFileBin))#32in
gdc4 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFileBin = loadFromFileBin))#64in
gdc5 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFileBin = loadFromFileBin))#128in
if loadFromFileBin:loadFromFileBin.close()

dlist = []
DF = 96             # Dimension of D filters in first conv layer. default [64]
loadFromFileBin = ConvNet.openBinaryFileR('gan11d.bin')
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*1,filterSize = 5,loadFromFileBin = loadFromFileBin))#128out
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFileBin = loadFromFileBin))#64out
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*2,filterSize = 5,loadFromFileBin = loadFromFileBin))#32out
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFileBin = loadFromFileBin))#16out
dcv4 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 3,loadFromFileBin = loadFromFileBin))#8out
dcv5 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*8,outDepth = DF*8,filterSize = 3,loadFromFileBin = loadFromFileBin))#4out
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*8*3*4,outDepth = 128,loadFromFileBin = loadFromFileBin))
dfc1 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = 128,outDepth = 1,loadFromFileBin = loadFromFileBin))
if loadFromFileBin:loadFromFileBin.close()
print("endload")

def generator(z):
    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, 4, 3, GF*8)
    _ret = gdc0.getLayer(_ret, height = 8, width = 6, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc1.getLayer(_ret, height = 16, width = 12, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc2.getLayer(_ret, height = 32, width = 24, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = 64, width = 48, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc4.getLayer(_ret, height = 128, width = 96, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc5.getLayer(_ret, height = IMAGE_H, width = IMAGE_W, convStride = 2, isRelu=False, fixed = False)
    return _ret
    
def discriminator(inputT):
    _ret = dcv0.getLayer(inputT, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv2.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv3.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv4.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv5.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=True, fixed = False)
    _ret = dfc1.getLayer(_ret, isRelu=False, fixed = False)
    return _ret


def train():
    ###################
    #loadedimage = CBHR.extract_data()
    #ConvNet.saveImages(loadedimage, [8, 8], "test0.png")
    #loadedimage = CBHR.extract_data()
    #ConvNet.saveImages(loadedimage, [8, 8], "test1.png")
    #exit()

    
    
#     for idx in xrange(0, 1000000000):
#         loadedimage = extract_data()
#         global file_index
#         global content_index
#         print(str(file_index)+","+str(content_index))         
#     exit()

    ###################
    
    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    z = tf.placeholder(tf.float32, [BATCH_SIZE, Z_DIM])
    
    G = generator(z)
    D_logits  = tf.reduce_mean(discriminator(images))
    D_logits_F = tf.reduce_mean(discriminator(G))
    
    gen_cost = -D_logits_F
    disc_cost = D_logits_F - D_logits

    alpha = tf.random_uniform(shape=[BATCH_SIZE,1], minval=0.0,maxval=1.0)
    differences = G - images
    differences = tf.reshape(differences,[BATCH_SIZE,-1])
    imagereshape = tf.reshape(images,[BATCH_SIZE,-1])
    interpolates = imagereshape + (alpha*differences)
    interpolates = tf.reshape(interpolates,images.shape)
    gradients = tf.gradients(discriminator(interpolates), [interpolates])[0]
    slopes = tf.sqrt(tf.reduce_sum(tf.square(gradients), reduction_indices=[1]))
    gradient_penalty = tf.reduce_mean((slopes-1.0)**2)
    
    LAMBDA = 10 # Gradient penalty lambda hyperparameter
    disc_cost += LAMBDA*gradient_penalty

    g_vars = ConvNet.getParam(glist)
    d_vars = ConvNet.getParam(dlist)

    LR = 0.000005         # Learning rate
    d_optim = tf.train.AdamOptimizer(learning_rate=0.000005,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=0.00001,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sess = tf.Session()

    sample_z1 = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    sample_z2 = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    #sample_z3 = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    #sample_z4 = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    #for i in xrange(0,BATCH_SIZE):
    #    sample_z[i] = np.random.uniform(-(i/BATCH_SIZE), (i/BATCH_SIZE), size = (Z_DIM))
    
    init = tf.global_variables_initializer()  
    sess.run(init)

    start_time = time.time()
    idx = -1
    while True:
        idx = idx + 1
        elapsed_time = int((time.time() - start_time)*1000)
        start_time = time.time()

        if idx % 200 == 0:

            sample = sess.run(G, feed_dict = {z: sample_z1})

            #sample = np.zeros([BATCH_SIZE*2, IMAGE_H, IMAGE_W, IMAGE_CHANNEL], dtype=np.float32)
            #sample[BATCH_SIZE*0:BATCH_SIZE*1] = sess.run(G, feed_dict = {z: sample_z1})
            #sample[BATCH_SIZE*1:BATCH_SIZE*2] = sess.run(G, feed_dict = {z: sample_z2})
            #sample[BATCH_SIZE*2:BATCH_SIZE*3] = sess.run(G, feed_dict = {z: sample_z3})
            #sample[BATCH_SIZE*3:BATCH_SIZE*4] = sess.run(G, feed_dict = {z: sample_z4})

            def imgSave(idx,sample):
                ConvNet.saveImages(sample, [2,5], 'out11\\sample_%d.png' % (idx))
                
                
            t = threading.Thread(target=imgSave,args=(idx,sample))
            t.start()

        dt = 0
        df = 0
        for _ in xrange(2):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage = CBHR.extract_data()
            _,d1,d2 = sess.run([d_optim,D_logits,D_logits_F], feed_dict = {z:batch_z, images:loadedimage})
            dt = dt + d1
            df = df + d2

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z})

        print(str(idx)+","+str(elapsed_time)+","+str(dt)+","+str(df)+","+str(dt-df))#

        
        exist = False

        if idx%10 == 0:
            exist = os.path.exists("stop.txt")
            
        if (idx!=0 and idx % 2000 == 0) or exist:
            os.system ("del stop.txt")
            def save(idx, gSaver, dSaver):
                print("start save")
                saveToFile = ConvNet.openBinaryFileW("gan11g"+str(idx)+".bin")
                for item in gSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
 
                saveToFile = ConvNet.openBinaryFileW("gan11d"+str(idx)+".bin")
                for item in dSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
                print("end save")
            
            gSaver = []
            dSaver = []
            for item in glist:
                gSaver.append(item.getSaver(sess, True))
            for item in dlist:
                dSaver.append(item.getSaver(sess, True))

            t = threading.Thread(target=save,args=(idx,gSaver, dSaver))
            t.start()
            
            
        if exist:
            break

    sess.close()
    

train()
