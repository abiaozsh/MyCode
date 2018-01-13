import numpy as np

import imageio
import scipy.misc
import random
import tensorflow as tf
import threading
import ConvNet
import time
import os
from six.moves import xrange

#训练检测眼口鼻

BATCH_SIZE = 16

IMAGE_W = 192
IMAGE_H = 256

DF = 32             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3
LR = 0.00001         # Learning rate
#左右翻转


def read_image(path):
    image = scipy.misc.imread(path)
    image = np.delete(image,3,2)
    
    if random.uniform(-1,1)>0 :
        image = np.flip(image, 1)
    
    n = random.uniform(0.7,1.3)
    
    IMAGE_H2 = int(IMAGE_H*n)
    IMAGE_W2 = int(IMAGE_W*n)
    
    image = scipy.misc.imresize(image,(IMAGE_H2,IMAGE_W2),interp="bilinear")

    r = random.uniform(-20,20)
    
    image = scipy.misc.imrotate(image,r)
    
    j1 = (IMAGE_H2//2) - (IMAGE_H//2)
    i1 = (IMAGE_W2//2) - (IMAGE_W//2)

    y1 = (IMAGE_H//2) - (IMAGE_H2//2) 
    x1 = (IMAGE_W//2) - (IMAGE_W2//2) 

    y2 = (IMAGE_H2//2) + (IMAGE_H//2)
    x2 = (IMAGE_W2//2) + (IMAGE_W//2)

    sx = int(random.uniform(-10,10))
    sy = int(random.uniform(-10,10))
    
    j1 = j1 + sy
    i1 = i1 + sx
    y1 = y1 - sy
    x1 = x1 - sx

    if j1 < 0 :j1=0
    if i1 < 0 :i1=0
    if y1 < 0 :y1=0
    if x1 < 0 :x1=0
    
    if y2 > IMAGE_H2-j1 :
        y2=IMAGE_H2-j1
    if x2 > IMAGE_W2-i1 :
        x2=IMAGE_W2-i1
    if y2 > IMAGE_H-y1 :
        y2=IMAGE_H-y1
    if x2 > IMAGE_W-x1 :
        x2=IMAGE_W-x1
        
    image2 = np.ndarray([IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)
    image2[:,:,:] = 128

    aa = image[j1:j1+y2,i1:i1+x2,:]
    
    image2[y1:y1+y2,x1:x1+x2,:] = aa

    image = image2/255.0 - 0.5
    
    return image


class celebaFacePretty:
    def __init__(self, _dir):
        self._dir = _dir
        self._file = os.listdir(_dir)
        self.nextImage = None
        self.t = None
    def extract_data(self):

        def _load_t():
            buff = np.ndarray([BATCH_SIZE, IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)
            for i in xrange(0,BATCH_SIZE):
                idx = int(random.uniform(0, len(self._file)))
                data = read_image(self._dir+self._file[idx])
                buff[i] = data
            #return buff
            self.nextImage = buff
            self.t = None
            
        _load_t()
        return self.nextImage

        while self.nextImage is None:
            if self.t and self.t.isAlive():
                self.t.join()
                print("join")
            else:
                self.t = threading.Thread(target=_load_t,args=())
                self.t.start()
                print("start")

        ret = self.nextImage
        self.nextImage = None
    
        self.t = threading.Thread(target=_load_t,args=())
        self.t.start()
    
        return ret

CBT = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\good\\")
CBF = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\nogood\\")
CBM = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\mid\\")
CBPL = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\plus\\")
CBMI = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\minus\\")

#img = CBT.extract_data()
#ConvNet.saveImages(img, [4,4], "test.jpg")
#exit()

dlist = []
loadFromFile = ConvNet.openEmptyFileR('gan10d.txt')
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*2,filterSize = 7,loadFromFile = loadFromFile))#64out
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFile = loadFromFile))#32out
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 5,loadFromFile = loadFromFile))#16out
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*8,outDepth = DF*16,filterSize = 3,loadFromFile = loadFromFile))#8out
dcv4 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*16,outDepth = DF*16,filterSize = 3,loadFromFile = loadFromFile))#4out
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*16*3*4,outDepth = 64,loadFromFile = loadFromFile))
dfc1 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = 64,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

def discriminator(inputT):
    _ret = dcv0.getLayer(inputT, convStride = 2, poolSize = 2,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv2.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv3.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv4.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=True, fixed = False)
    _ret = dfc1.getLayer(_ret, isRelu=False, fixed = False)
    return _ret


def train():
    ###################


#ConvNet.saveImages(images, [4,4], "test.jpg")
#     exit()

    ###################
    
    imagesT = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    imagesF = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    imagesM = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    imagesPL = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    imagesMI = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])

    DT = discriminator(imagesT)
    DF = discriminator(imagesF)
    DM = discriminator(imagesM)
    DPL = discriminator(imagesPL)
    DMI = discriminator(imagesMI)

    loss = (tf.reduce_mean(tf.nn.relu(  DF -  1))   + 
            tf.reduce_mean(tf.nn.relu(   1 - DT))   +
            tf.reduce_mean(tf.nn.relu(  DM -  1))   +
            tf.reduce_mean(tf.nn.relu(- DM -  1)) +
            tf.reduce_mean(tf.nn.relu(-DPL))     +
            tf.reduce_mean(tf.nn.relu( DMI))
             )
    
    #训练器
    #optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')  #这个 训练器 是用来处理分类的情况，现在是回归所以不用
    optimizer = tf.train.AdamOptimizer(learning_rate=0.0001,beta1=0.5,beta2=0.9).minimize(loss)
    #optimizer = tf.train.GradientDescentOptimizer(0.000001).minimize(loss)

    sess = tf.Session()

    init = tf.global_variables_initializer()  
    sess.run(init)
    
    zeros = 0
    
    start_time = time.time()
    idx = 0
    while True:
        idx = idx + 1

        ImagesT = CBT.extract_data()
        ImagesF = CBF.extract_data()
        ImagesM = CBM.extract_data()
        ImagesPL = CBPL.extract_data()
        ImagesMI = CBMI.extract_data()

        _,Loss = sess.run([optimizer,loss], feed_dict = {imagesT: ImagesT,imagesF: ImagesF,imagesM: ImagesM,imagesPL: ImagesPL,imagesMI: ImagesMI})

        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(Loss)+","+str(elapsed_time))
        
        if Loss == 0:
            zeros = zeros + 1
        else:
            zeros = 0
            
        if zeros >= 200 or idx%500==0:
            
            def save(idx, dSaver):
                print("start save")

                saveToFile = ConvNet.openEmptyFileW("gan10d"+str(idx)+".txt")
                for item in dSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
                print("end save")
            
            dSaver = []
            for item in dlist:
                dSaver.append(item.getSaver(sess))

            t = threading.Thread(target=save,args=(idx, dSaver))
            t.start()

        if zeros >= 200:
            break
        
        if idx >=20000:
            break

    sess.close()
    

train()
