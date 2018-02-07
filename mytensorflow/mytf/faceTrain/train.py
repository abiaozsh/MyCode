import numpy as np

import scipy.misc
import random
import tensorflow as tf
import threading
import ConvNet
import time
import os
from six.moves import xrange

#训练检测眼口鼻

BATCH_SIZE = 6

IMAGE_W = 192
IMAGE_H = 256

IMAGE_CHANNEL = 3



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

def getFullPath(_sname):
    i = int(_sname[0:6])
    folder1 = str(i // 10000);
    folder2 = str(i // 1000);
    folder3 = str(i // 100);
    dirs = "E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\"+folder1+"\\"+folder2+"\\"+folder3+"\\"+_sname;
    return dirs

class celebaFacePretty:
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
    
        #_load_t()
        #return self.nextImage

        while self.nextImage is None:
            if self.t and self.t.isAlive():
                self.t.join()
                print("join")
            else:
                self.t = threading.Thread(target=self._load_t,args=())
                self.t.start()
                print("start")

        ret = self.nextImage
        self.nextImage = None
    
        self.t = threading.Thread(target=self._load_t,args=())
        self.t.start()
    
        return ret

class celebaFacePretty2:
    def __init__(self, flg, _dir):
        self._dir = _dir
        self.flg = flg
        self._file = os.listdir(_dir)
        self.nextImage = None
        self.t = None
    def _load_t(self):
        buffG = np.ndarray([BATCH_SIZE, IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)
        buffB = np.ndarray([BATCH_SIZE, IMAGE_H,IMAGE_W,IMAGE_CHANNEL], np.float32)

        for i in xrange(0,BATCH_SIZE):
            idx = int(random.uniform(0, len(self._file)))
            
            _name = self._file[idx].split("_")
            
            
            _name0 = getFullPath(_name[0])
            _name1 = getFullPath(_name[1])
            
            if self.flg:
                dataG = read_image(_name0)
                dataB = read_image(_name1)
            else:
                dataG = read_image(_name1)
                dataB = read_image(_name0)
            
            buffG[i] = dataG
            buffB[i] = dataB
            
            
            
        #return buff
        self.nextImage = buffG,buffB
        self.t = None
    def extract_data(self):
            
        #_load_t()
        #return self.nextImage

        while self.nextImage is None:
            if self.t and self.t.isAlive():
                self.t.join()
                print("join")
            else:
                self.t = threading.Thread(target=self._load_t,args=())
                self.t.start()
                print("start")

        ret = self.nextImage
        self.nextImage = None
    
        self.t = threading.Thread(target=self._load_t,args=())
        self.t.start()
    
        return ret


CBT = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\good\\")
CBF = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\nogood\\")
CBM = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\mid\\")
CBPL = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\plus\\")
CBMI = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\minus\\")
#CB_GB =  celebaFacePretty2(True,"E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\GB\\")
CB_BG =  celebaFacePretty2(False,"E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\BG\\")

#img = CBT.extract_data()
#ConvNet.saveImages(img, [4,4], "test.jpg")
#exit()

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
    
    imagesG = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    imagesB = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])

    DT = discriminator(imagesT)
    DF = discriminator(imagesF)
    DM = discriminator(imagesM)
    DPL = discriminator(imagesPL)
    DMI = discriminator(imagesMI)
    DG = discriminator(imagesG)
    DB = discriminator(imagesB)

    loss = (tf.reduce_mean(tf.nn.relu(  DF -  1))   + 
            tf.reduce_mean(tf.nn.relu(   1 - DT))   +
            tf.reduce_mean(tf.nn.relu(  DM -  1))   +
            tf.reduce_mean(tf.nn.relu(- DM -  1)) +
            tf.reduce_mean(tf.nn.relu(-DPL))     +
            tf.reduce_mean(tf.nn.relu( DMI))   +
            tf.reduce_mean(tf.nn.relu(DB  - DG     ))
             )
    
    #训练器
    #optimizer = tf.train.AdadeltaOptimizer(learning_rate=1).minimize(loss)  # tf.train.AdadeltaOptimizer.init(learning_rate=0.001, rho=0.95, epsilon=1e-08, use_locking=False, name='Adadelta')  #这个 训练器 是用来处理分类的情况，现在是回归所以不用
    optimizer = tf.train.AdamOptimizer(learning_rate=0.0001,beta1=0.5,beta2=0.9).minimize(loss)
    #optimizer = tf.train.GradientDescentOptimizer(0.000001).minimize(loss)

    sess = tf.Session()

    init = tf.global_variables_initializer()  
    sess.run(init)
    
    zeros = 0
    
    def save(idx, dSaver):
        print("start save")

        saveToFile = ConvNet.openEmptyFileW("faceTrain"+str(idx)+".txt")
        for item in dSaver:
            item(saveToFile)
        saveToFile.flush();saveToFile.close()
        print("end save")
    
    start_time = time.time()
    idx = 0
    while True:
        idx = idx + 1

        ImagesT = CBT.extract_data()
        ImagesF = CBF.extract_data()
        ImagesM = CBM.extract_data()
        ImagesPL = CBPL.extract_data()
        ImagesMI = CBMI.extract_data()
        
        #if random.uniform(-1,1)>0 :
        #    ImagesG,ImagesB = CB_GB.extract_data()
        #else:
        ImagesG,ImagesB = CB_BG.extract_data()
        
        if False:
            img = np.ndarray([1, IMAGE_H, IMAGE_W, IMAGE_CHANNEL], np.float32)
            img[0] = ImagesG[0]
            ConvNet.saveImages(img, [1,1], "G.png")
            img[0] = ImagesB[0]
            ConvNet.saveImages(img, [1,1], "B.png")
            exit()
            
        _,Loss = sess.run([optimizer,loss], feed_dict = {imagesT: ImagesT,
                                                         imagesF: ImagesF,
                                                         imagesM: ImagesM,
                                                         imagesPL: ImagesPL,
                                                         imagesMI: ImagesMI,
                                                         imagesG: ImagesG,
                                                         imagesB: ImagesB,

                                                         })

        elapsed_time = int((time.time() - start_time)*1000)
        start_time = time.time()
        print(str(idx)+","+str(Loss)+","+str(elapsed_time))
        
        if Loss == 0:
            zeros = zeros + 1
        else:
            zeros = 0

        if idx%10 == 0:
            exist = os.path.exists("stop.txt")
            if exist:
                zeros = 200
                    
        if zeros >= 200 or idx%1000==0:
            
            dSaver = []
            for item in dlist:
                dSaver.append(item.getSaver(sess))

            t = threading.Thread(target=save,args=(idx, dSaver))
            t.start()

        if zeros >= 200:
            break
        
        if idx >=200000:
            break

    sess.close()
    

train()
