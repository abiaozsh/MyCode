import numpy as np

import scipy.misc
import random
import tensorflow as tf
import threading
import ConvNet
import time
import os
from six.moves import xrange

训练检测眼口鼻

BATCH_SIZE = 16

IMAGE_W = 192
IMAGE_H = 256

GF = 32             # Dimension of G filters in first conv layer. default [64]
DF = 32             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 100
IMAGE_CHANNEL = 3
LR = 0.0001         # Learning rate
#左右翻转

dirTrue = "K:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\done\\"
dirFalse = "K:\\MNIST\\CelebA\\Img\\img_celeba.7z\\NoFace\\"
fileTrue = os.listdir(dirTrue)
fileFalse = os.listdir(dirFalse)

def read_image(path):
    image = scipy.misc.imread(path)
    image = scipy.misc.imresize(image,(IMAGE_H,IMAGE_W))
    image = image[np.newaxis,:,:,:] 
    image = image.astype('float32')/255.0 - 0.5
    return image

def extract_TrueData():
    buff = np.ndarray([BATCH_SIZE, IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)
    for i in xrange(0,16):
        idx = int(random.uniform(0, len(fileTrue)))
        buff[i] = read_image(dirTrue+fileTrue[idx])
    return buff

def extract_FalseData():
    buff = np.ndarray([BATCH_SIZE, IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)
    for i in xrange(0,16):
        idx = int(random.uniform(0, len(fileFalse)))
        data = read_image(dirFalse+fileFalse[idx])
        data = np.delete(data,3,3)
        buff[i] = data
    return buff

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

    DT = discriminator(imagesT)
    DF = discriminator(imagesF)

    loss = tf.reduce_mean(DF) - tf.reduce_mean(DT)

    #训练器
    #optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')  #这个 训练器 是用来处理分类的情况，现在是回归所以不用
    optimizer = tf.train.AdamOptimizer(learning_rate=0.0001,beta1=0.5,beta2=0.9).minimize(loss)
    #optimizer = tf.train.GradientDescentOptimizer(0.000001).minimize(loss)

    sess = tf.Session()

    init = tf.global_variables_initializer()  
    sess.run(init)

    start_time = time.time()
    for idx in xrange(1, 50001):

        Lables = np.ndarray([BATCH_SIZE, 1], np.float32)
        Lables[0:8] = 1
        Lables[8:16] = -1
        ImagesT = extract_TrueData()
        ImagesF = extract_FalseData()
        
        _,Loss = sess.run([optimizer,loss], feed_dict = {imagesT: ImagesT,imagesF: ImagesF})

        if idx % 10 == 0:
            ImagesT = extract_TrueData()
            lbls = sess.run(DT, feed_dict = {imagesT: ImagesT})
            print(lbls)

            ImagesT = extract_FalseData()
            lbls = sess.run(DT, feed_dict = {imagesT: ImagesT})
            print(lbls)

        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(Loss)+","+str(elapsed_time))
        

        if idx % 500 == 0:
            
            def save(idx,  dSaver):
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
            
            
            

    sess.close()
    

train()
