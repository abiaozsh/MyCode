import os
#from six.moves import xrange

#训练检测眼口鼻

BATCH_SIZE = 6

IMAGE_W = 192
IMAGE_H = 256

IMAGE_CHANNEL = 3


class celebaFacePretty:
    def __init__(self, _dir):
        self._file = os.listdir(_dir+"\\")
        self.nextImage = None
        self.t = None
        
        out_file = open(_dir+".txt", 'w')
        
        for file in self._file:
            _name = file
            _name = _name[len(_name)-10:len(_name)]
            
            out_file.write(_name + "\n")
        
        out_file.close()

class celebaFacePretty2:
    def __init__(self, flg, _dir):
        self.flg = flg
        self._file = os.listdir(_dir+"\\")
        self.nextImage = None
        self.t = None
        
        out_file = open(_dir+".txt", 'w')

        for file in self._file:

            _name = file
            
            out_file.write(_name + "\n")
            
        out_file.close()
        
CBT = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\good")
CBF = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\nogood")
CBM = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\mid")
CBPL = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\plus")
CBMI = celebaFacePretty("E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\minus")
#CB_GB =  celebaFacePretty2(True,"E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\GB")
CB_BG =  celebaFacePretty2(False,"E:\\MNIST\\CelebA\\Img\\img_celeba.7z\\trainData\\BG")
