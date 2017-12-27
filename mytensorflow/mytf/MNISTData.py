import numpy as np


IMAGE_W = 28
IMAGE_H = 28
IMAGE_CHANNEL = 1

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
        labels = np.frombuffer(buf, dtype=np.uint8).astype(np.int64)
    return labels
    
train_data = loaddata('E:\\MNIST\\train-images.idx3-ubyte', 60000)
train_label = loadlabels('E:\\MNIST\\train-labels.idx1-ubyte', 60000)
test_data = loaddata('E:\\MNIST\\t10k-images.idx3-ubyte', 10000)
test_label = loadlabels('E:\\MNIST\\t10k-labels.idx1-ubyte', 10000)

train_index = 0
def extract_traindata(BATCH_SIZE):
    global train_index
    train_index = train_index + BATCH_SIZE
    if train_index>=60000:#202599
        train_index = 0

    return train_data[train_index:train_index+BATCH_SIZE],train_label[train_index:train_index+BATCH_SIZE]

test_index = 0
def extract_testdata():
    global test_index
    test_index = test_index + 1
    if test_index>=10000:#202599
        test_index = 0

    return test_data[test_index:test_index+1],test_label[test_index:test_index+1]


_data = np.concatenate((train_data,test_data),axis=0)
_label = np.concatenate((train_label,test_label),axis=0)

content_index = 0
def extract_data(BATCH_SIZE):
    global content_index
    content_index = content_index + BATCH_SIZE
    if content_index>=70000:#202599
        content_index = 0

    return _data[content_index:content_index+BATCH_SIZE],_label[content_index:content_index+BATCH_SIZE]



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
    