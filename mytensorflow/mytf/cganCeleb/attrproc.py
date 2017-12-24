import numpy as np

import threading
import ConvNet
import time

from six.moves import xrange

keylist = [
"index"                
,"5_o_Clock_Shadow"     
,"Arched_Eyebrows"      
,"Attractive"           
,"Bags_Under_Eyes"      
,"Bald"                 
,"Bangs"                
,"Big_Lips"             
,"Big_Nose"             
,"Black_Hair"           
,"Blond_Hair"           
,"Blurry"               
,"Brown_Hair"           
,"Bushy_Eyebrows"       
,"Chubby"               
,"Double_Chin"          
,"Eyeglasses"           
,"Goatee"               
,"Gray_Hair"            
,"Heavy_Makeup"         
,"High_Cheekbones"      
,"Male"                 
,"Mouth_Slightly_Open"  
,"Mustache"             
,"Narrow_Eyes"          
,"No_Beard"             
,"Oval_Face"            
,"Pale_Skin"            
,"Pointy_Nose"          
,"Receding_Hairline"    
,"Rosy_Cheeks"          
,"Sideburns"            
,"Smiling"              
,"Straight_Hair"        
,"Wavy_Hair"            
,"Wearing_Earrings"     
,"Wearing_Hat"          
,"Wearing_Lipstick"     
,"Wearing_Necklace"     
,"Wearing_Necktie"      
,"Young"                
,"lefteye_x"            
,"lefteye_y"            
,"righteye_x"           
,"righteye_y"           
,"nose_x"
,"nose_y"        
,"leftmouth_x"          
,"leftmouth_y"          
,"rightmouth_x"         
,"rightmouth_y"         
]


total = 202599
disk = "K:"
loadFromFileattr = ConvNet.openEmptyFileR(disk+"\\MNIST\\CelebA\\Anno\\list_attr_celeba.txt")
loadFromFileldmk = ConvNet.openEmptyFileR(disk+"\\MNIST\\CelebA\\Anno\\list_landmarks_align_celeba.txt")

line = loadFromFileattr.readline();
line = loadFromFileattr.readline();
line = loadFromFileldmk.readline();
line = loadFromFileldmk.readline();

alist = []
avg = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
top = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]
btn = [0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0]

for i in xrange(0,total):
    attr = loadFromFileattr.readline()
    ldmk = loadFromFileldmk.readline()
    if i%10000==0:
        print(i)
    attr = attr.replace("  "," ")
    ldmk = ldmk.replace("  "," ")
    attr = attr.replace("  "," ")
    ldmk = ldmk.replace("  "," ")
    attr = attr.replace("  "," ")
    ldmk = ldmk.replace("  "," ")
    
    attr = attr.split(" ")
    ldmk = ldmk.split(" ")
    a = {}
    
    a[keylist[0]] = i+1
    a[keylist[1]] = attr[1]
    a[keylist[2]] = attr[2]
    a[keylist[3]] = attr[3]
    a[keylist[4]] = attr[4]
    a[keylist[5]] = attr[5]
    a[keylist[6]] = attr[6]
    a[keylist[7]] = attr[7]
    a[keylist[8]] = attr[8]
    a[keylist[9]] = attr[9]
    a[keylist[10]] = attr[10]
    a[keylist[11]] = attr[11]
    a[keylist[12]] = attr[12]
    a[keylist[13]] = attr[13]
    a[keylist[14]] = attr[14]
    a[keylist[15]] = attr[15]
    a[keylist[16]] = attr[16]
    a[keylist[17]] = attr[17]
    a[keylist[18]] = attr[18]
    a[keylist[19]] = attr[19]
    a[keylist[20]] = attr[20]
    a[keylist[21]] = attr[21]
    a[keylist[22]] = attr[22]
    a[keylist[23]] = attr[23]
    a[keylist[24]] = attr[24]
    a[keylist[25]] = attr[25]
    a[keylist[26]] = attr[26]
    a[keylist[27]] = attr[27]
    a[keylist[28]] = attr[28]
    a[keylist[29]] = attr[29]
    a[keylist[30]] = attr[30]
    a[keylist[31]] = attr[31]
    a[keylist[32]] = attr[32]
    a[keylist[33]] = attr[33]
    a[keylist[34]] = attr[34]
    a[keylist[35]] = attr[35]
    a[keylist[36]] = attr[36]
    a[keylist[37]] = attr[37]
    a[keylist[38]] = attr[38]
    a[keylist[39]] = attr[39]
    a[keylist[40]] = attr[40]
    a[keylist[41]] = ldmk[1]
    a[keylist[42]] = ldmk[2]
    a[keylist[43]] = ldmk[3]
    a[keylist[44]] = ldmk[4]
    a[keylist[45]] = ldmk[5]
    a[keylist[46]] = ldmk[6]
    a[keylist[47]] = ldmk[7]
    a[keylist[48]] = ldmk[8]
    a[keylist[49]] = ldmk[9]
    a[keylist[50]] = ldmk[10]
    
    if a["Blurry"] == "1":
        print(a["index"])
    
    for j in xrange(1,51):
        a[keylist[j]] = float(a[keylist[j]])

    for j in xrange(0,10):
        avg[j] = avg[j] + a[keylist[41+j]]

    alist.append(a)

for j in xrange(0,10):
    avg[j] = avg[j] / total

for i in xrange(0,total):
    a = alist[i]
    for j in xrange(0,10):
        a[keylist[41+j]] = a[keylist[41+j]] - avg[j]
        if a[keylist[41+j]] > top[j]:
            top[j] = a[keylist[41+j]]
        if a[keylist[41+j]] < btn[j]:
            btn[j] = a[keylist[41+j]]

for i in xrange(0,total):
    a = alist[i]
    for j in xrange(0,10):
        a[keylist[41+j]] = a[keylist[41+j]] / max( abs(top[j]) , abs(btn[j]) )

    
report = str(alist[0]).replace(",", ",\n")
print(report)

report = str(alist[1]).replace(",", ",\n")
print(report)

    