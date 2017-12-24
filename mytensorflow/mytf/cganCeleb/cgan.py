import numpy as np

import tensorflow as tf
import threading
import ConvNet
import time

from six.moves import xrange


keylist = [#50
"5_o_Clock_Shadow"      #0
,"Arched_Eyebrows"      #1
,"Attractive"           #2
,"Bags_Under_Eyes"      #3
,"Bald"                 #4
,"Bangs"                #5
,"Big_Lips"             #6
,"Big_Nose"             #7
,"Black_Hair"           #8
,"Blond_Hair"           #9
,"Blurry"               #10
,"Brown_Hair"           #11
,"Bushy_Eyebrows"       #12
,"Chubby"               #13
,"Double_Chin"          #14
,"Eyeglasses"           #15
,"Goatee"               #16
,"Gray_Hair"            #17
,"Heavy_Makeup"         #18
,"High_Cheekbones"      #19
,"Male"                 #20
,"Mouth_Slightly_Open"  #21
,"Mustache"             #22
,"Narrow_Eyes"          #23
,"No_Beard"             #24
,"Oval_Face"            #25
,"Pale_Skin"            #26
,"Pointy_Nose"          #27
,"Receding_Hairline"    #28
,"Rosy_Cheeks"          #29
,"Sideburns"            #30
,"Smiling"              #31
,"Straight_Hair"        #32
,"Wavy_Hair"            #33
,"Wearing_Earrings"     #34
,"Wearing_Hat"          #35
,"Wearing_Lipstick"     #36
,"Wearing_Necklace"     #37
,"Wearing_Necktie"      #38
,"Young"                #39
,"lefteye_x"            #40
,"lefteye_y"            #41
,"righteye_x"           #42
,"righteye_y"           #43
,"nose_x"               #44
,"nose_y"               #45
,"leftmouth_x"          #46
,"leftmouth_y"          #47
,"rightmouth_x"         #48
,"rightmouth_y"         #49
]



BATCH_SIZE = 16
saveSize = [2, 8]

IMAGE_W = 160
IMAGE_H = 208

GF = 96             # Dimension of G filters in first conv layer. default [64]
DF = 96             # Dimension of D filters in first conv layer. default [64]
Z_DIM = 50
V_DIM = 50

IMAGE_CHANNEL = 3
LR = 0.0001         # Learning rate



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


label_index = 0
filePath = "e:\\MNIST\\celebaATTR.txt"
labelstream = open(filePath,"r")
def extract_label():
    global label_index
    global labelstream

    label_index = label_index + BATCH_SIZE
    if label_index>=4096*49:#202599
        labelstream.close()
        labelstream = open(filePath,"r")
        label_index = 0

    data = np.ndarray([BATCH_SIZE,V_DIM], np.float32)
    for i in xrange(0,BATCH_SIZE):
        for j in xrange(0,V_DIM):
            line = labelstream.readline()
            data[i,j] = float(line)
    return data


t2, t4, t8, t16 = IMAGE_H//2, IMAGE_H//4, IMAGE_H//8, IMAGE_H//16
s2, s4, s8, s16 = IMAGE_W//2, IMAGE_W//4, IMAGE_W//8, IMAGE_W//16


glist = []
loadFromFile = ConvNet.openEmptyFileR('gan0g.txt')
gfc0 = ConvNet.addlist(glist,ConvNet.FC(inDepth = Z_DIM+V_DIM,outDepth = Z_DIM+V_DIM,loadFromFile = loadFromFile))
gdc0 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*8,outDepth = GF*4,filterSize = 5,loadFromFile = loadFromFile))
gdc1 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*4,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile))
gdc2 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = GF*2,filterSize = 5,loadFromFile = loadFromFile))
gdc3 = ConvNet.addlist(glist,ConvNet.DeConv(inDepth = GF*2,outDepth = IMAGE_CHANNEL,filterSize = 5,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

dlist = []
loadFromFile = ConvNet.openEmptyFileR('gan0d.txt')
dcva = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL+V_DIM,outDepth = DF*1,filterSize = 1,loadFromFile = loadFromFile))
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF*1,filterSize = 5,loadFromFile = loadFromFile))
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*1,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFile = loadFromFile))
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 5,loadFromFile = loadFromFile))
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*2*t4*s4,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

def generator(z, y_label):
    cat1 = tf.concat([z, y_label], 1)
    _ret = gfc0.getLayer(cat1, isRelu=True, fixed = False)
    _ret = ConvNet.FC2Conv_Reshape(_ret, t4, s4, GF*2)
    _ret = gdc0.getLayer(_ret, height = t8, width = s8, convStride = 2, isRelu=True, fixed = False)
    _ret = gdc1.getLayer(_ret, height = t4, width = s4, convStride = 2, isRelu=True, fixed = False)
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
