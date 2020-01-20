#include <stdio.h>
#include "HelloWorld.h"
#include "wiringPi.c"

#define  REQ 27
#define  ACK 26
#define  RST 25
#define  DAT 24
//CMD[23:16]

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


        if         (rbp_cmd==0)begin//reset address
          command<=8'hB0;
        end else if(rbp_cmd==1)begin//read
          command<=8'hB1;
        end else if(rbp_cmd==2)begin//get pos low
          command<=8'hB2;
        end else if(rbp_cmd==3)begin//get pos high
          command<=8'hB3;
        end else if(rbp_cmd==4)begin//fetch
          command<=8'hB4;
        end else if(rbp_cmd==5)begin//start
          command<=8'hB5;
        end else if(rbp_cmd==6)begin//stop
          command<=8'hB6;
        end else if(rbp_cmd==15)begin//test
          command<=8'hBF;

*/
static volatile unsigned int* gpio13;
static volatile unsigned int* gpio10;
static volatile unsigned int* gpio7;

void setCmd(int cmd){
  digitalWrite(16, (cmd & 0x01)?HIGH:LOW);
  digitalWrite(17, (cmd & 0x02)?HIGH:LOW);
  digitalWrite(18, (cmd & 0x04)?HIGH:LOW);
  digitalWrite(19, (cmd & 0x08)?HIGH:LOW);
  digitalWrite(20, (cmd & 0x10)?HIGH:LOW);
  digitalWrite(21, (cmd & 0x20)?HIGH:LOW);
  digitalWrite(22, (cmd & 0x40)?HIGH:LOW);
  digitalWrite(23, (cmd & 0x80)?HIGH:LOW);
  delay(1);
}
void exec(){
  REQHIGH;//digitalWrite(REQ, HIGH);
  while(!PINACK);// digitalRead(ACK)==LOW
  short data = (short)(*(gpio + 13));//read16();
  REQLOW;//digitalWrite(REQ, LOW);
  while(PINACK);//digitalRead(ACK)==HIGH
}

JNIEXPORT jint JNICALL Java_HelloWorld_init(JNIEnv* env, jclass obj){
  volatile unsigned int* gpio = wiringPiSetup();
  gpio13 = gpio + 13;
  gpio10 = gpio + 10;
  gpio7 = gpio + 7;
  
  for(int i=0;i<32;i++){
    pinMode(i, INPUT);
  }
  
  pinMode(REQ, OUTPUT);
  pinMode(RST, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);

  digitalWrite(REQ, LOW);
  digitalWrite(RST, HIGH);
  delay(1);
  digitalWrite(RST, LOW);

  return 0;
}

JNIEXPORT jint JNICALL Java_HelloWorld_dumpData(JNIEnv* env, jclass obj, jbyteArray buffA, jbyteArray buffB){
  //GetIntArrayElements
  jbyte* arrA = (*env)->GetByteArrayElements(env, buffA, NULL);
  jbyte* arrB = (*env)->GetByteArrayElements(env, buffB, NULL);

  setCmd(0x00);//reset address
  exec();
  setCmd(0x01);
  for(int i=0;i<16*1024*1024;i++){
    REQHIGH;//digitalWrite(REQ, HIGH);
    while(!PINACK);// digitalRead(ACK)==LOW
    short data = (short)(*(gpio + 13));//read16();
    arrA[i]=data&0xFF;
    arrB[i]=(data>>8)&0xFF;
    REQLOW;//digitalWrite(REQ, LOW);
    while(PINACK);//digitalRead(ACK)==HIGH
  }
  
  int pos;
  {
    //get pos low
    setCmd(0x02);
    REQHIGH;//digitalWrite(REQ, HIGH);
    while(!PINACK);// digitalRead(ACK)==LOW
    short data = (short)(*(gpio + 13));//read16();
    pos = data;
    REQLOW;//digitalWrite(REQ, LOW);
    while(PINACK);//digitalRead(ACK)==HIGH
  }
  
  
  {
    //get pos high
    setCmd(0x03);
    REQHIGH;//digitalWrite(REQ, HIGH);
    while(!PINACK);// digitalRead(ACK)==LOW
    short data = (short)(*(gpio + 13));//read16();
    int tmp = (data<<16)&0xFFFF;
    pos = data|tmp;
    REQLOW;//digitalWrite(REQ, LOW);
    while(PINACK);//digitalRead(ACK)==HIGH
  }

  (*env)->ReleaseByteArrayElements(env, buffA, arrA, JNI_COMMIT);
  (*env)->ReleaseByteArrayElements(env, buffB, arrB, JNI_COMMIT);
  
  return (jint)pos;
}

JNIEXPORT jint JNICALL Java_HelloWorld_fetchData(JNIEnv* env, jclass obj){
  setCmd(0x04);
  REQHIGH;//digitalWrite(REQ, HIGH);
  while(!PINACK);// digitalRead(ACK)==LOW
  short data = (short)(*(gpio + 13));//read16();
  REQLOW;//digitalWrite(REQ, LOW);
  while(PINACK);//digitalRead(ACK)==HIGH
  return (int)data;
}
JNIEXPORT jint JNICALL Java_HelloWorld_startRecord(JNIEnv* env, jclass obj){
  setCmd(0x05);
  exec();
}
JNIEXPORT jint JNICALL Java_HelloWorld_stopRecord(JNIEnv* env, jclass obj){
  setCmd(0x06);
  exec();
}


//    end else if(rbp_cmd==7)begin//start fetch frame
//    command<=8'hB7;
//  end else if(rbp_cmd==8)begin//stop fetch frame
//    command<=8'hB8;
//  end else if(rbp_cmd==9)begin//fetch frame
//    command<=8'hB9;
//  end else if(rbp_cmd==10)begin//reset frame rate max
//    command<=8'hBA;
//  end else if(rbp_cmd==11)begin//inc frame rate
//    command<=8'hBB;

JNIEXPORT jint JNICALL Java_HelloWorld_setFrameRate(JNIEnv* env, jclass obj, jint val){

  setCmd(0x0A);//reset frame rate max
  exec();

  for(int i=0;i<(int)val;i++){
    setCmd(0x0B);//inc frame rate
    exec();
  }
}

JNIEXPORT jint JNICALL Java_HelloWorld_fetchFrame(JNIEnv* env, jclass obj, jbyteArray frameA, jbyteArray frameB){
  //GetIntArrayElements
  jbyte* arrA = (*env)->GetByteArrayElements(env, frameA, NULL);
  jbyte* arrB = (*env)->GetByteArrayElements(env, frameB, NULL);

  setCmd(0x07);//start fetch frame
  exec();
  setCmd(0x09);//fetch frame
  for(int i=0;i<1024;i++){
    REQHIGH;//digitalWrite(REQ, HIGH);
    while(!PINACK);// digitalRead(ACK)==LOW
    short data = (short)(*(gpio + 13));//read16();
    arrA[i]=data&0xFF;
    arrB[i]=(data>>8)&0xFF;
    REQLOW;//digitalWrite(REQ, LOW);
    while(PINACK);//digitalRead(ACK)==HIGH
  }

  
  setCmd(0x08);//stop fetch frame
  exec();

  
  
  (*env)->ReleaseByteArrayElements(env, frameA, arrA, JNI_COMMIT);
  (*env)->ReleaseByteArrayElements(env, frameB, arrB, JNI_COMMIT);

  return 0;
}


//JNIEXPORT jint JNICALL Java_HelloWorld_test(JNIEnv* env, jclass obj);
JNIEXPORT jint JNICALL Java_HelloWorld_test(JNIEnv* env, jclass obj){
  
  setCmd(0x0F);
  REQHIGH;//digitalWrite(REQ, HIGH);
  while(!PINACK);// digitalRead(ACK)==LOW
  short data = (short)(*(gpio + 13));//read16();
  REQLOW;//digitalWrite(REQ, LOW);
  while(PINACK);//digitalRead(ACK)==HIGH
  return (jint)data;
}




/*
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
}*/

