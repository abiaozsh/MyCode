import numpy as np

import scipy.io
import scipy.misc
import random
import tensorflow as tf
import threading
import ConvNet
import time
import os
from six.moves import xrange
from ConvNet import Conv

BATCH_SIZE = 1

IMAGE_W = 192
IMAGE_H = 256

GF = 32             # Dimension of G filters in first conv layer. default [64]
DF = 32             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3
LR = 0.0001         # Learning rate
#左右翻转

def read_image(path):
    image = scipy.misc.imread(path)
    image = scipy.misc.imresize(image,(IMAGE_H,IMAGE_W))
    image = image[np.newaxis,:,:,:] 
    image = image.astype('float32')/255.0 - 0.5
    return image

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
    
    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])

    D = discriminator(images)

    sess = tf.Session()

    init = tf.global_variables_initializer()  
    sess.run(init)
    
    for i in xrange(0,21):
        dirs = "K:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\"+str(i)+"\\"
        files = os.listdir(dirs)
    
        for file in files:
    
            img = read_image(dirs+file)
    
            lbl = sess.run(D, feed_dict = {images: img})
    
            print(lbl)
            
            if lbl <0.5: 
                os.system ("move %s %s" % (dirs+file, "K:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\garbage\\"))

    sess.close()
    

train()
