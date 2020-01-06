#include <stdio.h>
#include "HelloWorld.h"
#include "wiringPi.c"

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

static int vvv;

int test0(int val){
  
  vvv = val;
}

int test1(){
  
  return vvv;
}

//JNIEXPORT jint JNICALL Java_HelloWorld_sayHello
//  (JNIEnv *, jclass, jint);

int mydigitalRead (int pin)
{

    if ((*(gpio + 13) & (1 << (pin & 31))) != 0)
      return HIGH ;
    else
      return LOW ;
}

void mydigitalWrite (int pin, int value)
{

    if (value == LOW)
      *(gpio + 10) = 1 << (pin & 31) ;
    else
      *(gpio + 7) = 1 << (pin & 31) ;
}


void setCmd(volatile unsigned int* gpio,int cmd){
  mydigitalWrite(gpio, CMD0, (cmd & 0x01)?HIGH:LOW);
  mydigitalWrite(gpio, CMD1, (cmd & 0x02)?HIGH:LOW);
  mydigitalWrite(gpio, CMD2, (cmd & 0x04)?HIGH:LOW);
  mydigitalWrite(gpio, CMD3, (cmd & 0x08)?HIGH:LOW);
}

int init(){
  
  
  
  volatile unsigned int* gpio = wiringPiSetup();
  
    for(int i=0;i<32;i++){
    pinMode(i, INPUT);
  }
  
  pinMode(REQ, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(CMD0, OUTPUT);
  pinMode(CMD1, OUTPUT);
  pinMode(CMD2, OUTPUT);
  pinMode(CMD3, OUTPUT);

  digitalWrite(REQ, LOW);
  digitalWrite(RST, HIGH);
  digitalWrite(RST, LOW);

  return (long)gpio;
}

int test(){
  
  setCmd(volatile unsigned int* gpio,int cmd){
}

#define PINACK ((*gpio13) & 1 << ACK)
#define REQHIGH *(gpio7) = 1 << (REQ)
#define REQLOW  *(gpio10) = 1 << (REQ)

int dump1(){
  volatile unsigned int* gpio = ??;
  
  volatile unsigned int* gpio13 = gpio + 13;
  volatile unsigned int* gpio10 = gpio + 10;
  volatile unsigned int* gpio7 = gpio + 7;

    for(int i=0;i<pagesize;i++){
      REQHIGH;//digitalWrite(REQ, HIGH);
      while(!PINACK);// digitalRead(ACK)==LOW
      short data = (short)(*(gpio + 13));//read16();
      buff[j*pagesize+i]=data;
      REQLOW;//digitalWrite(REQ, LOW);
      while(PINACK);//digitalRead(ACK)==HIGH
    }
    printf("%d\r\n",j);

}