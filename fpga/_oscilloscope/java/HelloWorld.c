#include <stdio.h>
#include "HelloWorld.h"

//find / -name "jni.h"
// /usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/jni.h

//find / -name "jni_md.h"
// /usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux/jni_md.h

//gcc -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/ -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux -fPIC -shared HelloWorld.c -o libHelloWorld.so


JNIEXPORT jint JNICALL Java_HelloWorld_sayHello(JNIEnv* env, jobject obj, jshortArray arr)
{
  //GetIntArrayElements
  jshort* elems = (*env)->GetShortArrayElements(env, arr, NULL);
  elems[2] = 0x3344;
  elems[3] = 0x5566;
  
        printf("%d",elems[0]);
        printf("%d",elems[1]);
        printf("Hello World in C!\n");
        
        jshort val = elems[10];
        
        //ReleaseIntArrayElements
        (*env)->ReleaseShortArrayElements(env, arr, elems, JNI_COMMIT);
        
        return val;
}

//JNIEXPORT jint JNICALL Java_HelloWorld_sayHello
//  (JNIEnv *, jclass, jint);