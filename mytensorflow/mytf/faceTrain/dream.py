
import numpy as np

import tensorflow as tf
from six.moves import xrange
import ConvNet

BATCH_SIZE = 1

IMAGE_W = 192
IMAGE_H = 256
IMAGE_CHANNEL = 3


# creating TensorFlow session and loading the model

t_input = tf.placeholder(np.float32, name='input') # define the input tensor

def strip_consts(graph_def, max_const_size=32):
    """Strip large constant values from graph_def."""
    strip_def = tf.GraphDef()
    for n0 in graph_def.node:
        n = strip_def.node.add() 
        n.MergeFrom(n0)
        if n.op == 'Const':
            tensor = n.attr['value'].tensor
            size = len(tensor.tensor_content)
            if size > max_const_size:
                tensor.tensor_content = tf.compat.as_bytes("<stripped %d bytes>"%size)
    return strip_def
  
def rename_nodes(graph_def, rename_func):
    res_def = tf.GraphDef()
    for n0 in graph_def.node:
        n = res_def.node.add() 
        n.MergeFrom(n0)
        n.name = rename_func(n.name)
        for i, s in enumerate(n.input):
            n.input[i] = rename_func(s) if s[0]!='^' else '^'+rename_func(s[1:])
    return res_def




DF = 32             # Dimension of D filters in first conv layer. default [64]
dlist = []
loadFromFile = ConvNet.openTextFileR('faceTrain.txt')
dcv0 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = IMAGE_CHANNEL,outDepth = DF,filterSize = 5,loadFromFile = loadFromFile))#64out
dcv1 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF,outDepth = DF*2,filterSize = 5,loadFromFile = loadFromFile))#64out
dcv2 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*2,outDepth = DF*4,filterSize = 5,loadFromFile = loadFromFile))#32out
dcv3 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*4,outDepth = DF*8,filterSize = 5,loadFromFile = loadFromFile))#16out
dcv4 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*8,outDepth = DF*16,filterSize = 3,loadFromFile = loadFromFile))#8out
dcv5 = ConvNet.addlist(dlist,ConvNet.Conv(inDepth = DF*16,outDepth = DF*16,filterSize = 3,loadFromFile = loadFromFile))#4out
dfc0 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = DF*16*3*4,outDepth = 128,loadFromFile = loadFromFile))
dfc1 = ConvNet.addlist(dlist,ConvNet.FC(inDepth = 128,outDepth = 1,loadFromFile = loadFromFile))
if loadFromFile:loadFromFile.close()

imagesT = tf.placeholder(tf.float32, [BATCH_SIZE, IMAGE_H, IMAGE_W, IMAGE_CHANNEL])
l0 = dcv0.getLayer(imagesT, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
l1 = dcv1.getLayer(l0, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
l2 = dcv2.getLayer(l1, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
l3 = dcv3.getLayer(l2, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
l4 = dcv4.getLayer(l3, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
l5 = dcv5.getLayer(l4, convStride = 1, poolSize = 2,isRelu=True, fixed = False)
l6 = ConvNet.Conv2FC_Reshape(l5)
l7 = dfc0.getLayer(l6, isRelu=True, fixed = False)
l8 = dfc1.getLayer(l7, isRelu=False, fixed = False)





# Picking some internal layer. Note that we use outputs before applying the ReLU nonlinearity
# to have non-zero gradients for features with negative initial activations.

# start with a gray image with a little noise
img_noise = np.random.uniform(-0.01,0.01,size=(BATCH_SIZE,IMAGE_H,IMAGE_W,IMAGE_CHANNEL))

def visstd(a, s=0.1):
    '''Normalize the image range for visualization'''
    return (a-a.mean())/max(a.std(), 1e-4)*s + 0.5

sess = tf.Session()
init = tf.global_variables_initializer()  
sess.run(init)

def render_naive(idx,_name,t_obj, img0=img_noise, iter_n=200, step=0.0001):
    t_score = tf.reduce_mean(t_obj) # defining the optimization objective
    t_grad = tf.gradients(t_score, imagesT)[0] # behold the power of automatic differentiation!
    
    img = img0.copy()
    for i in range(iter_n):
        g, score = sess.run([t_grad, t_score], {imagesT:img})
        # normalizing the gradient, so the same step size should work 
        g /= g.std()+1e-8         # for different layers and networks
        img += g*step
        print(score)
    
    ConvNet.saveImages(img, [1,1], _name+'demo'+str(idx)+'.png')

def read_image(path):
    image = scipy.misc.imread(path)
    #image = scipy.misc.imresize(image,(IMAGE_H,IMAGE_W))
    image = image[np.newaxis,:,:,:]
    if(image.shape[3]==4):
        image = np.delete(image,3,3)
    image = image.astype('float32')/255.0 - 0.5
    return image

#for i in xrange(0,DF*2):
    #render_naive(i,"l0",l0[:,:,:,i])

#for i in xrange(0,DF*4):
    #render_naive(i,"l1",l1[:,:,:,i])

#for i in xrange(0,DF*8):
    #render_naive(i,"l2",l2[:,:,:,i])

#for i in xrange(0,DF*16):
#    render_naive(i,"l3",l3[:,:,:,i])

#for i in xrange(0,DF*16):
#    render_naive(i,"l4",l4[:,:,:,i])
img = read_image("badtogood.png")


render_naive(0,"l8",l8[:,0],img0=img)