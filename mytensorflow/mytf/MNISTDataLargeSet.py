import numpy as np

IMAGE_CHANNEL = 1

IMAGE_W = 28
IMAGE_H = 28

content_index = 0
filePath = "e:\\MNIST\\MNISTLargeSet"
bytestreamDat = open(filePath + "Dat.bin","br")
def extract_data(BATCH_SIZE):
    global content_index
    global bytestreamDat

    content_index = content_index + BATCH_SIZE
    if content_index>=2000000-BATCH_SIZE:#202599
        bytestreamDat.close()
        bytestreamDat = open(filePath + "Dat.bin","br")
        content_index = 0

    buf = bytestreamDat.read(BATCH_SIZE * IMAGE_H * IMAGE_W * IMAGE_CHANNEL)
    data = np.frombuffer(buf, dtype=np.uint8).astype(np.float32)
    data = (data) / 256.0 - 0.5
    data = data.reshape(BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL)

    return data

label_index = 0
bytestreamLbl = open(filePath + "Lbl.bin","br")
def extract_label(BATCH_SIZE):
    global label_index
    global bytestreamLbl

    label_index = label_index + BATCH_SIZE
    if label_index>=2000000-BATCH_SIZE:#202599
        bytestreamLbl.close()
        bytestreamLbl = open(filePath + "Lbl.bin","br")
        label_index = 0

    buf = bytestreamLbl.read(BATCH_SIZE)
    label = np.frombuffer(buf, dtype=np.uint8).astype(np.int64)
    
    return label
    
    
        ##################################
#         saveSize = [1, 256]
#         loadedimage = extract_data()
#         lbl1 = extract_label()
#         lbl2 = sess.run(_test, feed_dict={testOne:loadedimage})
#         ConvNet.saveImagesMono(loadedimage, saveSize, "test0.png")
#         print(lbl1,np.argmax(lbl2, 1))
# 
#         exit()
        ##################################
