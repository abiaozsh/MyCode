import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time

from six.moves import xrange

BATCH_SIZE = 8
saveSize = [2, 4]

IMAGE_W = 160
IMAGE_H = 208

GF = 96             # Dimension of G filters in first conv layer. default [64]
DF = 96             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 128
IMAGE_CHANNEL = 3
LR = 0.0001
start = 1
end = 100001


file_index = 0
content_index = 0
filePath = "e:\\MNIST\\celebaHI\\"
bytestream = open(filePath + str(file_index)+".bin","br")
def extract_data():
    global file_index
    global content_index
    global bytestream

    content_index = content_index + BATCH_SIZE
    if content_index>=4096:#202599
        file_index = file_index + 1
        if file_index >= 49:
            file_index = 0
        bytestream.close()
        bytestream = open(filePath + str(file_index)+".bin","br")
        content_index = 0

    buf = bytestream.read(BATCH_SIZE * IMAGE_H * IMAGE_W * IMAGE_CHANNEL)
    data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
    data = (data) / 256.0 - 0.5
    data = data.reshape(BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL)
    return data

t2, t4, t8, t16 = IMAGE_H//2, IMAGE_H//4, IMAGE_H//8, IMAGE_H//16
s2, s4, s8, s16 = IMAGE_W//2, IMAGE_W//4, IMAGE_W//8, IMAGE_W//16

print("loading")
glist = []
loadFromFile = ConvNet.openEmptyFileR("gan10g"+str(start)+".txt")
gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM,outDepth = GF*8*t16*s16,loadFromFile = loadFromFile))
gdc0 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 5,loadFromFile = loadFromFile))
gdc1 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile))
gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile))
gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

dlist = []
loadFromFile = ConvNet.openEmptyFileR("gan10d"+str(start)+".txt")
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile))
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFile = loadFromFile))
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 5,loadFromFile = loadFromFile))
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*8*t16*s16,outDepth = Z_DIM,loadFromFile = loadFromFile))
dfc1 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = Z_DIM,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()
print("loaded")

def generator(z):
    _ret = gfc0.getLayer(z, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, t16, s16, GF*8)
    _ret = gdc0.getLayer(_ret, height = t8, width = s8, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc1.getLayer(_ret, height = t4, width = s4, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc2.getLayer(_ret, height = t2, width = s2, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc3.getLayer(_ret, height = IMAGE_H, width = IMAGE_W, convStride = 2, isRelu=False, fixed = False)
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

#def sampler(z):
#    return generator(z)


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
    
    #testz = tf.placeholder(tf.float32, [testBATCH_SIZE, Z_DIM])
    
    G = generator(z)
    D_logits  = discriminator(images)
    D_logits_F = discriminator(G)

    gen_cost = -tf.reduce_mean(D_logits_F)
    disc_cost = tf.reduce_mean(D_logits_F) - tf.reduce_mean(D_logits)
    alpha = tf.random_uniform(shape=[BATCH_SIZE,1], minval=0.0,maxval=1.0)
    differences = G - images
    differences = tf.reshape(differences,[BATCH_SIZE,-1])
    imagereshape = tf.reshape(images,[BATCH_SIZE,-1])
    interpolates = imagereshape + (alpha*differences)
    interpolates = tf.reshape(interpolates,images.shape)
    gradients = tf.gradients(discriminator(interpolates), [interpolates])[0]
    slopes = tf.sqrt(tf.reduce_sum(tf.square(gradients), reduction_indices=[1]))
    gradient_penalty = tf.reduce_mean((slopes-1.)**2)
    
    LAMBDA = 10 # Gradient penalty lambda hyperparameter
    disc_cost += LAMBDA*gradient_penalty

    g_vars = ConvNet.getParam(glist)
    d_vars = ConvNet.getParam(dlist)

    global_step = tf.Variable(0, trainable=False)
    #LR = tf.train.exponential_decay(learning_rate=0.01,global_step= global_step, decay_steps=1000,decay_rate= 0.95,staircase=True)
        #starter_learning_rate, global_step, 100000, 0.96, staircase=True)
        
    d_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(disc_cost, var_list=d_vars)        
    g_optim = tf.train.AdamOptimizer(learning_rate=LR,beta1=0.5,beta2=0.9).minimize(gen_cost, var_list=g_vars)


    sess = tf.Session()
    
    sample_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
    
    init = tf.global_variables_initializer()  
    sess.run(init)

    start_time = time.time()
    for idx in xrange(start, end):

        for _ in xrange(2):
            batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
            loadedimage = extract_data()
            _,genCost,discCost,globalStep = sess.run([d_optim,gen_cost,disc_cost,global_step], feed_dict = {z:batch_z, images:loadedimage})

        global file_index
        global content_index
        elapsed_time = time.time() - start_time
        start_time = time.time()
        print(str(idx)+","+str(file_index)+","+str(content_index)+","+str(elapsed_time),","+str(genCost)+","+str(discCost)+","+str(globalStep))

        batch_z = np.random.uniform(-1, 1, size = (BATCH_SIZE, Z_DIM))
        sess.run(g_optim, feed_dict = {z: batch_z})

        if idx % 50 == 0 or idx==10:

            sample = sess.run(G, feed_dict = {z: sample_z})

            def imgSave(idx,sample):
                ConvNet.saveImages(sample, saveSize, 'out\\sample_%d.png' % (idx))
                
                
            t = threading.Thread(target=imgSave,args=(idx,sample))
            t.start()
            
        if idx % 500 == 0 or idx==10:
            
            def save(idx, gSaver, dSaver):
                print("start save")
                saveToFile = ConvNet.openEmptyFileW("gan10g"+str(idx)+".txt")
                for item in gSaver:
                    item(saveToFile)
                saveToFile.flush();saveToFile.close()
 
                saveToFile = ConvNet.openEmptyFileW("gan10d"+str(idx)+".txt")
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
