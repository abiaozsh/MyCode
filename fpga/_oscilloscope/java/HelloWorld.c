#include <stdio.h>
#include "HelloWorld.h"
#include "wiringPi.c"

#define  CTL1P3  21
#define  CTL2R3  20
#define  CTL3R4  26
#define  CTL4R5  19
#define  CTL5R6  25
#define  CTL6R7  22
#define  CTL7R8  27
#define  CTL8R9  17

#define  REQ CTL7R8
#define  ACK CTL8R9
#define  RST CTL6R7
#define  DAT CTL5R6

#define  CMD0 CTL1P3
#define  CMD1 CTL2R3
#define  CMD2 CTL3R4
#define  CMD3 CTL4R5

#define PINACK ((*gpio13) & 1 << ACK)
#define REQHIGH *(gpio7) = 1 << (REQ)
#define REQLOW  *(gpio10) = 1 << (REQ)

//find / -name "jni.h"
// /usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/jni.h

//find / -name "jni_md.h"
// /usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux/jni_md.h

//gcc -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/ -I/usr/lib/jvm/jdk-8-oracle-arm32-vfp-hflt/include/linux -fPIC -shared HelloWorld.c -o libHelloWorld.so

/*
sudo mount tmpfs /mnt/tmpfs -t tmpfs -o size=50m
cd /mnt/tmpfs

sudo chmod 777 run.sh
*/


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


void setCmd(int cmd){
  digitalWrite(CMD0, (cmd & 0x01)?HIGH:LOW);
  digitalWrite(CMD1, (cmd & 0x02)?HIGH:LOW);
  digitalWrite(CMD2, (cmd & 0x04)?HIGH:LOW);
  digitalWrite(CMD3, (cmd & 0x08)?HIGH:LOW);
}

static volatile unsigned int* gpio13;
static volatile unsigned int* gpio10;
static volatile unsigned int* gpio7;

JNIEXPORT jint JNICALL Java_HelloWorld_init(JNIEnv* env, jclass elems){
  volatile unsigned int* gpio = wiringPiSetup();
  gpio13 = gpio + 13;
  gpio10 = gpio + 10;
  gpio7 = gpio + 7;
  
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

  return 0;
}

JNIEXPORT jint JNICALL Java_HelloWorld_test(JNIEnv* env, jclass elems){
  
  setCmd(0x0F);
  REQHIGH;//digitalWrite(REQ, HIGH);
  while(!PINACK);// digitalRead(ACK)==LOW
  short data = (short)(*(gpio + 13));//read16();
  REQLOW;//digitalWrite(REQ, LOW);
  while(PINACK);//digitalRead(ACK)==HIGH
  return (jint)data;
}
/*
#define PINACK ((*gpio13) & 1 << ACK)
#define REQHIGH *(gpio7) = 1 << (REQ)
#define REQLOW  *(gpio10) = 1 << (REQ)

int dump1(){
  volatile unsigned int* gpio = ??;
  

    for(int i=0;i<pagesize;i++){
      REQHIGH;//digitalWrite(REQ, HIGH);
      while(!PINACK);// digitalRead(ACK)==LOW
      short data = (short)(*(gpio + 13));//read16();
      buff[j*pagesize+i]=data;
      REQLOW;//digitalWrite(REQ, LOW);
      while(PINACK);//digitalRead(ACK)==HIGH
    }
    printf("%d\r\n",j);

}*/