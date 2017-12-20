# google deepdream tensorflow
# http://blog.csdn.net/matrix_space/article/details/53419973
# http://blog.csdn.net/matrix_space/article/details/53419548
# http://blog.csdn.net/matrix_space/article/details/53418478
# http://blog.topspeedsnail.com/archives/10667
# 
# 
# transfer
# http://blog.csdn.net/matrix_space/article/details/54286086
# http://blog.csdn.net/matrix_space/article/details/54290460
# http://blog.csdn.net/matrix_space/article/details/54409864
# 
# 
# http://blog.csdn.net/shinian1987/article/list/4
# 
# 
# 
# 
# 
# https://github.com/paarthneekhara/text-to-image
# 
# 
# 
# https://open_nsfw.gitlab.io/
# 
# 
# http://press.liacs.nl/mirflickr/mirdownload.html



# -*- coding: utf-8 -*-

# boilerplate code
from __future__ import print_function
import os
from io import BytesIO
import numpy as np
from functools import partial
import PIL.Image
from IPython.display import clear_output, Image, display, HTML

import tensorflow as tf
import scipy.misc




model_fn = 'e:\\MNIST\\tensorflow_inception_graph.pb'

# creating TensorFlow session and loading the model
graph = tf.Graph()
sess = tf.InteractiveSession(graph=graph)
with tf.gfile.FastGFile(model_fn, 'rb') as f:
    graph_def = tf.GraphDef()
    graph_def.ParseFromString(f.read())
t_input = tf.placeholder(np.float32, name='input') # define the input tensor
imagenet_mean = 117.0
t_preprocessed = tf.expand_dims(t_input-imagenet_mean, 0)
tf.import_graph_def(graph_def, {'input':t_preprocessed})


#tf.nn.avg_pool(value, ksize, strides, padding, data_format, name)
#tf.concat(values, axis, name)




# 
# for vv in graph.get_operations():
#     #if vv.type!="Conv2D":
#     #    continue
#     cnt = -1
#     name = vv.name
#     print(dir(vv))
#     layer = graph.get_tensor_by_name(name+':0')
#     #layer1 = graph.get_tensor_by_name(name+':1')
#     #['_InputList', '__class__', '__delattr__', '__dict__', '__dir__', '__doc__', '__eq__', '__format__', '__ge__', '__getattribute__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__le__', '__lt__', '__module__', '__ne__', '__new__', '__reduce__', '__reduce_ex__', '__repr__', '__setattr__', '__sizeof__', '__str__', '__subclasshook__', '__weakref__', '_add_control_input', '_add_control_inputs', '_add_input', '_c_op', '_control_flow_context', '_control_inputs', '_create_c_op', '_get_control_flow_context', '_graph', '_id', '_id_value', '_input_dtypes', '_input_types', '_input_types_val', '_inputs', '_node_def', '_op_def', '_original_op', '_output_types', '_output_types_val', '_outputs', '_recompute_node_def', '_reconstruct_sequence_inputs', '_set_control_flow_context', '_set_device', '_tf_input', '_tf_output', '_traceback', '_update_input', 'colocation_groups', 'control_inputs', 'device', 'get_attr', 'graph', 'inputs', 'name', 'node_def', 'op_def', 'outputs', 'run', 'traceback', 'traceback_with_start_lines', 'type', 'values']
#     #['OVERLOADABLE_OPERATORS', '__abs__', '__add__', '__and__', '__array_priority__', '__bool__', '__class__', '__delattr__', '__dict__', '__dir__', '__div__', '__doc__', '__eq__', '__floordiv__', '__format__', '__ge__', '__getattribute__', '__getitem__', '__gt__', '__hash__', '__init__', '__init_subclass__', '__invert__', '__iter__', '__le__', '__lt__', '__matmul__', '__mod__', '__module__', '__mul__', '__ne__', '__neg__', '__new__', '__nonzero__', '__or__', '__pow__', '__radd__', '__rand__', '__rdiv__', '__reduce__', '__reduce_ex__', '__repr__', '__rfloordiv__', '__rmatmul__', '__rmod__', '__rmul__', '__ror__', '__rpow__', '__rsub__', '__rtruediv__', '__rxor__', '__setattr__', '__sizeof__', '__str__', '__sub__', '__subclasshook__', '__truediv__', '__weakref__', '__xor__', '_add_consumer', '_as_node_def_input', '_as_tf_output', '_consumers', '_dtype', '_dup', '_handle_data', '_id', '_op', '_override_operator', '_shape', '_shape_as_list', '_shape_tuple', '_value_index', 'consumers', 'device', 'dtype', 'eval', 'get_shape', 'graph', 'name', 'op', 'set_shape', 'shape', 'value_index']
#     print(dir(layer))
#     print(layer.__add__)
#     #print(dir(layer1))
# #     try:
# #         name = vv.name
# #         cnt = str(int(graph.get_tensor_by_name(name+':0').get_shape()[-1]))
# #     except:
# #         a=1
# #     print(vv.name,vv.type,cnt)
# #layers = [op.name for op in graph.get_operations() if op.type=='Conv2D' and 'import/' in op.name]
# #feature_nums = [int(graph.get_tensor_by_name(name+':0').get_shape()[-1]) for name in layers]
# 
# 
# exit()

# Helper functions for TF Graph visualization

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
  
def show_graph(graph_def, max_const_size=32):
    """Visualize TensorFlow graph."""
    if hasattr(graph_def, 'as_graph_def'):
        graph_def = graph_def.as_graph_def()
    strip_def = strip_consts(graph_def, max_const_size=max_const_size)
    code = """
        <script>
          function load() {{
            document.getElementById("{id}").pbtxt = {data};
          }}
        </script>
        <link rel="import" href="https://tensorboard.appspot.com/tf-graph-basic.build.html" onload=load()>
        <div style="height:600px">
          <tf-graph-basic id="{id}"></tf-graph-basic>
        </div>
    """.format(data=repr(str(strip_def)), id='graph'+str(np.random.rand()))
  
    iframe = """
        <iframe seamless style="width:800px;height:620px;border:0" srcdoc="{}"></iframe>
    """.format(code.replace('"', '&quot;'))
    display(HTML(iframe))

# Visualizing the network graph. Be sure expand the "mixed" nodes to see their 
# internal structure. We are going to visualize "Conv2D" nodes.
#tmp_def = rename_nodes(graph_def, lambda s:"/".join(s.split('_',1)))
#show_graph(tmp_def)

















# Picking some internal layer. Note that we use outputs before applying the ReLU nonlinearity
# to have non-zero gradients for features with negative initial activations.

# start with a gray image with a little noise
img_noise = np.random.uniform(size=(128,128,3))*0.2 + 128.0

def showarray(a, path):
    a = np.uint8(np.clip(a, 0, 1)*255)
    scipy.misc.imsave(path, a)
    #f = BytesIO()
    #PIL.Image.fromarray(a).save(f, fmt)
    #display(Image(data=f.getvalue()))
    
def visstd(a, s=0.1):
    '''Normalize the image range for visualization'''
    return (a-a.mean())/max(a.std(), 1e-4)*s + 0.5

def T2(layer):
    '''Helper for getting layer output tensor'''
    return graph.get_tensor_by_name(layer+":0")

def render_naive(t_obj, img0=img_noise, iter_n=10, step=1.0):
    t_score = tf.reduce_mean(t_obj) # defining the optimization objective
    t_grad = tf.gradients(t_score, t_input)[0] # behold the power of automatic differentiation!
    
    img = img0.copy()
    for i in range(iter_n):
        g, score = sess.run([t_grad, t_score], {t_input:img})
        # normalizing the gradient, so the same step size should work 
        g /= g.std()+1e-8         # for different layers and networks
        img += g*step
        print(score, end = ' ')
    clear_output()
    showarray(visstd(img),"_0_render_naive.jpg")


#render_naive(T2("import/head1_bottleneck_pre_relu/conv")[:,:,:,1])



#








def tffunc(*argtypes):
    '''Helper that transforms TF-graph generating function into a regular one.
    See "resize" function below.
    '''
    placeholders = list(map(tf.placeholder, argtypes))
    def wrap(f):
        out = f(*placeholders)
        def wrapper(*args, **kw):
            return out.eval(dict(zip(placeholders, args)), session=kw.get('session'))
        return wrapper
    return wrap

# Helper function that uses TF to resize an image
def resize(img, size):
    img = tf.expand_dims(img, 0)
    return tf.image.resize_bilinear(img, size)[0,:,:,:]
resize = tffunc(np.float32, np.int32)(resize)


def calc_grad_tiled(img, t_grad, tile_size=512):
    '''Compute the value of tensor t_grad over the image in a tiled way.
    Random shifts are applied to the image to blur tile boundaries over 
    multiple iterations.'''
    
    
    g = sess.run(t_grad, {t_input:img})
    return g
    
    sz = tile_size
    h, w = img.shape[:2]
    sx, sy = np.random.randint(sz, size=2)
    img_shift = np.roll(np.roll(img, sx, 1), sy, 0)
    grad = np.zeros_like(img)
    for y in range(0, max(h-sz//2, sz),sz):
        for x in range(0, max(w-sz//2, sz),sz):
            print("shift")
            sub = img_shift[y:y+sz,x:x+sz]
            print("calc"+str(x)+","+str(y), end=' ')
            g = sess.run(t_grad, {t_input:sub})
            print("done")
            grad[y:y+sz,x:x+sz] = g
            print("grad")
    return np.roll(np.roll(grad, -sx, 1), -sy, 0)






def render_multiscale(t_obj, img0=img_noise, iter_n=10, step=1.0, octave_n=4, octave_scale=2):
    t_score = tf.reduce_mean(t_obj) # defining the optimization objective
    t_grad = tf.gradients(t_score, t_input)[0] # behold the power of automatic differentiation!
    
    img = img0.copy()
    for octave in range(octave_n):
        if octave>0:
            hw = np.float32(img.shape[:2])*octave_scale
            img = resize(img, np.int32(hw))
        for i in range(iter_n):
            g = calc_grad_tiled(img, t_grad)
            # normalizing the gradient, so the same step size should work 
            g /= g.std()+1e-8         # for different layers and networks
            img += g*step
            print('.', end = ' ')

        showarray(visstd(img),"_1_render_multiscale"+str(octave)+".jpg")

# aaa = T2("import/mixed3a_pool_reduce_pre_relu/conv")
# print(aaa.shape)
# bbb = aaa[:,:,:,5]
# render_multiscale(bbb)
# 
# 
# 
# exit()
















k = np.float32([1,4,6,4,1])
k = np.outer(k, k)
k5x5 = k[:,:,None,None]/k.sum()*np.eye(3, dtype=np.float32)

def lap_split(img):
    '''Split the image into lo and hi frequency components'''
    with tf.name_scope('split'):
        lo = tf.nn.conv2d(img, k5x5, [1,2,2,1], 'SAME')
        lo2 = tf.nn.conv2d_transpose(lo, k5x5*4, tf.shape(img), [1,2,2,1])
        hi = img-lo2
    return lo, hi

def lap_split_n(img, n):
    '''Build Laplacian pyramid with n splits'''
    levels = []
    for i in range(n):
        img, hi = lap_split(img)
        levels.append(hi)
    levels.append(img)
    return levels[::-1]

def lap_merge(levels):
    '''Merge Laplacian pyramid'''
    img = levels[0]
    for hi in levels[1:]:
        with tf.name_scope('merge'):
            img = tf.nn.conv2d_transpose(img, k5x5*4, tf.shape(hi), [1,2,2,1]) + hi
    return img

def normalize_std(img, eps=1e-10):
    '''Normalize image by making its standard deviation = 1.0'''
    with tf.name_scope('normalize'):
        std = tf.sqrt(tf.reduce_mean(tf.square(img)))
        return img/tf.maximum(std, eps)

def lap_normalize(img, scale_n=4):
    '''Perform the Laplacian pyramid normalization.'''
    img = tf.expand_dims(img,0)
    tlevels = lap_split_n(img, scale_n)
    tlevels = list(map(normalize_std, tlevels))
    out = lap_merge(tlevels)
    return out[0,:,:,:]

# Showing the lap_normalize graph with TensorBoard
# lap_graph = tf.Graph()
# with lap_graph.as_default():
#     lap_in = tf.placeholder(np.float32, name='lap_in')
#     lap_out = lap_normalize(lap_in)
# show_graph(lap_graph)


















def render_lapnorm(t_obj, img0=img_noise, visfunc=visstd, iter_n=10, step=1.0, octave_n=3, octave_scale=1.4, lap_n=4):
    t_score = tf.reduce_mean(t_obj) # defining the optimization objective
    t_grad = tf.gradients(t_score, t_input)[0] # behold the power of automatic differentiation!
    # build the laplacian normalization graph
    lap_norm_func = tffunc(np.float32)(partial(lap_normalize, scale_n=lap_n))

    img = img0.copy()
    for octave in range(octave_n):
        if octave>0:
            hw = np.float32(img.shape[:2])*octave_scale
            img = resize(img, np.int32(hw))
        for i in range(iter_n):
            g = calc_grad_tiled(img, t_grad)
            g = lap_norm_func(g)
            img += g*step
            print('.', end = ' ')
        clear_output()
        showarray(visfunc(img),"_2_render_lapnorm.jpg")

#render_lapnorm(T(layer)[:,:,:,channel])
#render_lapnorm(T(layer)[:,:,:,65])
#render_lapnorm(T('mixed3b_1x1_pre_relu')[:,:,:,101])
#render_lapnorm(T(layer)[:,:,:,65]+T(layer)[:,:,:,139], octave_n=4)




def T(layer):
    # Helper for getting layer output tensor
    return graph.get_tensor_by_name("import/%s:0"%layer)



def render_deepdream(t_obj, img0=img_noise, iter_n=10, step=1.5, octave_n=8, octave_scale=1.4):
    t_score = tf.reduce_mean(t_obj) # defining the optimization objective
    t_grad = tf.gradients(t_score, t_input)[0] # behold the power of automatic differentiation!

    # split the image into a number of octaves
    img = img0
    octaves = []
    for i in range(octave_n-1):
        hw = img.shape[:2]
        lo = resize(img, np.int32(np.float32(hw)/octave_scale))
        hi = img-resize(lo, hw)
        img = lo
        octaves.append(hi)
    
    # generate details octave by octave
    for octave in range(octave_n):
        if octave>0:
            hi = octaves[-octave]
            img = resize(img, hi.shape[:2])+hi
        for i in range(iter_n):
            g = calc_grad_tiled(img, t_grad)
            img += g*(step / (np.abs(g).mean()+1e-7))
            print('.',end = ' ')
        clear_output()
        showarray(img/255.0,"_3_render_deepdream.jpg")




img0 = PIL.Image.open('..\\..\\day.jpg')
img0 = np.float32(img0)
showarray(img0/255.0,"_4_img.jpg")

render_deepdream(T('mixed4c_pool_reduce_pre_relu')[:,:,:,12], img0)        
#render_deepdream(T(layer)[:,:,:,139], img0)

