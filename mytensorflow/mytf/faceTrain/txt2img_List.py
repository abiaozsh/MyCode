import numpy as np
import os
import ConvNet

import scipy.misc

BATCH_SIZE = 6

IMAGE_W = 192
IMAGE_H = 256

IMAGE_CHANNEL = 3

def read_image(path):
    image = scipy.misc.imread(path)
    #image = scipy.misc.imresize(image,(IMAGE_H,IMAGE_W))
    image = image[np.newaxis,:,:,:]
    if(image.shape[3]==4):
        image = np.delete(image,3,3)
    image = image.astype('float32')/255.0 - 0.5
    return image

def getFullPath(_sname):
    i = int(_sname[0:6])
    folder1 = str(i // 10000)
    folder2 = str(i // 1000)
    folder3 = str(i // 100)
    dirs = "E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\img_celebaProc\\"+folder1+"\\"+folder2+"\\"+folder3+"\\"+_sname
    return dirs

class celebaFacePretty:
    def __init__(self, _dir):
        os.system ("mkdir \"%s\"" % (_dir))
        
        list_file = open(_dir+".txt", 'r')
        
        while 1:
            line = list_file.readline()
            if not line:
                break
            
            line = line.strip()
            
            in_file = getFullPath(line)
            
            out_file = _dir+"\\"+line
            
            os.system ("copy \"%s\" \"%s\"" % (in_file, _dir+"\\"))
           
        list_file.close()

class celebaFacePretty2:
    def __init__(self, flg, _dir):
        os.system ("mkdir \"%s\"" % (_dir))
        
        list_file = open(_dir+".txt", 'r')
        
        while 1:
            line = list_file.readline()
            if not line:
                break
            
            line = line.strip()
            
            _name = line.split("_")
            
            in_file1 = getFullPath(_name[0])
            in_file2 = getFullPath(_name[1])
            
            out_file1 = _dir+"\\"+line

            img = np.ndarray([2, IMAGE_H, IMAGE_W, IMAGE_CHANNEL], np.float32)
            
            img[0] = read_image(in_file1)
            img[1] = read_image(in_file2)
            
            ConvNet.saveImages(img, [1,2], out_file1)
            
        list_file.close()
       
CBT = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\good")
CBF = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\nogood")
CBM = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\mid")
CBPL = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\plus")
CBMI = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\minus")
#CB_GB =  celebaFacePretty2(True,"E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\GB")
CB_BG =  celebaFacePretty2(False,"E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\BG")
