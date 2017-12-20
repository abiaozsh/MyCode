import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time

from six.moves import xrange

BATCH_SIZE = 200
testBATCH_SIZE = 40
saveSize = [4, 10]

IMAGE_W = 28
IMAGE_H = 28

GF = 32             # Dimension of G filters in first conv layer. default [64]
DF = 32             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 10
V_DIM = 10

IMAGE_CHANNEL = 1
LR = 0.0002         # Learning rate

def loaddata(filename, num_images):
    with open(filename,"br") as bytestream:
        bytestream.read(16)
        buf = bytestream.read(IMAGE_H * IMAGE_W * num_images * IMAGE_CHANNEL)
        data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
        data = (data) / 256.0 - 0.5
        data = data.reshape(num_images, IMAGE_H, IMAGE_W, IMAGE_CHANNEL)
        return data

def loadlabels(filename, num_images):
    with open(filename,"br") as bytestream:
        bytestream.read(8)
        buf = bytestream.read(num_images)
        labels = np.ndarray([num_images,V_DIM], np.float32)
        for idx in xrange(0,len(buf)):
            for x in xrange(0,V_DIM):
                if x==buf[idx]:
                    labels[idx,x] = 1
                else:
                    labels[idx,x] = -1
    return labels
    
train_data = loaddata('E:\\MNIST\\train-images.idx3-ubyte', 60000)
test_data = loaddata('E:\\MNIST\\t10k-images.idx3-ubyte', 10000)
train_labels = loadlabels('E:\\MNIST\\train-labels.idx1-ubyte', 60000)
test_labels = loadlabels('E:\\MNIST\\t10k-labels.idx1-ubyte', 10000)

_data = np.concatenate((train_data,test_data),axis=0)
_label = np.concatenate((train_labels,test_labels),axis=0)

content_index = 0
def extract_data():
    global content_index
    content_index = content_index + BATCH_SIZE
    if content_index>=70000:#202599
        content_index = 0

    return _data[content_index:content_index+BATCH_SIZE],_label[content_index:content_index+BATCH_SIZE]

t2, t4 = IMAGE_H//2, IMAGE_H//4
s2, s4 = IMAGE_W//2, IMAGE_W//4

glist = []
loadFromFile = ConvNet.openEmptyFileR('gan0g.txt')
gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM+V_DIM,outDepth = Z_DIM+V_DIM,loadFromFile = loadFromFile))
gfc1 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM+V_DIM,outDepth = GF*2*t4*s4,loadFromFile = loadFromFile))
gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*1,filterSize = 5,loadFromFile = loadFromFile))
gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*1,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

dlist = []
loadFromFile = ConvNet.openEmptyFileR('gan0d.txt')
dcva = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL+V_DIM,outDepth = DF*1,filterSize = 1,loadFromFile = loadFromFile))
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile))
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*2*t4*s4,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

def generator(z, y_label):
    cat1 = tf.concat([z, y_label], 1)
    _ret = gfc0.getLayer(cat1, isRelu=True, fixed = False)
    _ret = gfc1.getLayer(_ret, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, t4, s4, GF*2)
    _ret = gdc2.getLayer(_ret, height = t2, width = s2, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = IMAGE_H, width = IMAGE_W, convStride = 2, isRelu=False, fixed = False)
    return _ret
    
def discriminator(inputT, y_fill):
    cat1 = tf.concat([inputT, y_fill], 3)
    _ret = dcva.getLayer(cat1, convStride = 1, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv0.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = dcv1.getLayer(_ret, convStride = 2, poolSize = 1,isRelu=True, fixed = False)
    _ret = ConvNet.Conv2FC_Reshape(_ret)
    _ret = dfc0.getLayer(_ret, isRelu=False, fixed = False)
    return _ret

def train():
    ###################
#     loadedimage = extract_data()
#     ConvNet.saveImagesMono(loadedimage, saveSize, "test0.png")
#     loadedimage = extract_data()
#     ConvNet.saveImagesMono(loadedimage, saveSize, "test1.png")
#     exit()

    
    
#     for idx in xrange(0, 1000000000):
#         loadedimage = extract_data()
#         global file_index
#         global content_index
#         print(str(file_index)+","+str(content_index))         
#     exit()

    ###################
    
    images = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
    y_fill = tf.placeholder(tf.float32, shape=(BATCH_SIZE, IMAGE_H, IMAGE_W, V_DIM))
    
    z = tf.placeholder(tf.float32, [BATCH_SIZE, Z_DIM])
    y_label = tf.placeholder(tf.float32, shape=(BATCH_SIZE, V_DIM))
    
    testz = tf.placeholder(tf.float32, [testBATCH_SIZE, Z_DIM])
    test_label = tf.placeholder(tf.float32, shape=(testBATCH_SIZE, V_DIM))
    
    G = generator(z,y_label)
    D_logits  = discriminator(images,y_fill)
    samples = generator(testz,test_label)
    D_logits_F = discriminator(G,y_fill)

    gen_cost = -tf.reduce_mean(D_logits_F)
    disc_cost = tf.reduce_mean(D_logits_F) - tf.reduce_mean(D_logits)
    alpha = tf.random_uniform(shape=[BATCH_SIZE,1], minval=0.0,maxval=1.0)
    differences = G - images
    differences = tf.reshape(differences,[BATCH_SIZE,-1])
    imagereshape = tf.reshape(images,[BATCH_SIZE,-1])
    interpolates = imagereshape + (alpha*differences)
    interpolates = tf.reshape(interpolates,images.shape)
    gradients = tf.gradients(discriminator(interpolates,y_fill), [interpolates])[0]
    slopes = tf.sqrt(tf.reduce_sum(tf.square(gradients), reduction_indices=[1]))
    gradient_penalty = tf.reduce_mean((slopes-1.)**2)
    
    LAMBDA = 10 # Gradient penalty lambda hyperparameter
    disc_cost += LAMBDA*gradient_penalty

    g_vars = ConvNet.getParam(glist)
    d_vars = ConvNet.getParam(dlist)

    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)

    sample_z = np.random.uniform(-1, 1, size = (testBATCH_SIZE, Z_DIM))
    testLabel = np.ndarray([testBATCH_SIZE,V_DIM], np.float32)
    for j in xrange(0,4):
        for i in xrange(0,10):
            testLabel[j*10+i] = -1
            testLabel[j*10+i,i] = 1

    sess = tf.Session()
    
    init = tf.initialize_all_variables()  
    sess.run(init)

    start_time = time.time()
    for idx in xrange(0, 50001):
        global content_index
        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(content_index)+","+str(elapsed_time))
        
        for _ in xrange(2):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage,yLabel = extract_data()
            
            yFill = yLabel.reshape([BATCH_SIZE, 1, 1, 10]) * np.ones([BATCH_SIZE, IMAGE_H, IMAGE_W, V_DIM])

            sess.run(d_optim, feed_dict = {z:batch_z,y_label:yLabel,y_fill: yFill, images:loadedimage})

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z,y_label:yLabel,y_fill: yFill})

        if idx % 50 == 0:

            sample = sess.run(samples, feed_dict = {testz: sample_z,test_label:testLabel})

            def imgSave(idx,sample):
                ConvNet.saveImagesMono(sample, saveSize, 'out\\sample_%d.png' % (idx))
                
                
            t = threading.Thread(target=imgSave,args=(idx,sample))
            t.start()
            
        if idx % 500 == 0:
            
            def save(idx, gSaver, dSaver):
                print("start save")
                saveToFile = ConvNet.openEmptyFileW("gan0g"+str(idx)+".txt")
                for item in gSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
 
                saveToFile = ConvNet.openEmptyFileW("gan0d"+str(idx)+".txt")
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
            
            
            

    sess.close()
    

train()
