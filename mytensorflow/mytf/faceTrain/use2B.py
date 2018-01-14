import numpy as np

import scipy.misc
import tensorflow as tf
import ConvNet
import os
from six.moves import xrange
import random

BATCH_SIZE = 1

IMAGE_W = 192
IMAGE_H = 256

DF = 32             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3

def read_image(path):
    image = scipy.misc.imread(path)
    #image = scipy.misc.imresize(image,(IMAGE_H,IMAGE_W))
    image = image[np.newaxis,:,:,:]
    if(image.shape[3]==4):
        image = np.delete(image,3,3)
    image = image.astype('float32')/255.0 - 0.5
    return image

DF = 32             # Dimension of D filters in first conv layer. default [64]
dlist = []
loadFromFile = ConvNet.openEmptyFileR('faceTrain.txt')
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF,filterSize = 5,loadFromFile = loadFromFile))#64out
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))#64out
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFile = loadFromFile))#32out
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 5,loadFromFile = loadFromFile))#16out
dcv4 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*8,outDepth = DF*16,filterSize = 3,loadFromFile = loadFromFile))#8out
dcv5 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*16,outDepth = DF*16,filterSize = 3,loadFromFile = loadFromFile))#4out
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*16*3*4,outDepth = 128,loadFromFile = loadFromFile))
dfc1 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = 128,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

def discriminator(inputT):
    _ret = dcv0.getLayer(inputT, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = dcv2.getLayer(_ret, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = dcv3.getLayer(_ret, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = dcv4.getLayer(_ret, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = dcv5.getLayer(_ret, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=True, fixed = False)
    _ret = dfc1.getLayer(_ret, isRelu=False, fixed = False)
    return _ret

def getImg(i):
    folder1 = str(i // 10000);
    folder2 = str(i // 1000);
    folder3 = str(i // 100);

    dirs = "E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\"+folder1+"\\"+folder2+"\\"+folder3+"\\";
    files = os.listdir(dirs)
    
    filename = files[i % 100]
    
    file = dirs+filename
    
    return read_image(file),filename

def train():

    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])

    D = discriminator(images)

    sess = tf.Session()

    init = tf.global_variables_initializer()  
    sess.run(init)
    
    for idx in xrange(0,1000):
        print(idx)
        i1 = int(random.uniform(1,202599))

        img1,file = getImg(i1)

        lbl = sess.run(D, feed_dict = {images: img1})

        val = int((100 - lbl) * 200)

        ConvNet.saveImages(img1, [1,1], "E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\"+str(val)+"-"+str(lbl)+"-"+file)

    sess.close()
    

train()
