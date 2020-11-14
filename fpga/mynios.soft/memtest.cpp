#include "inc/io.h"
#include "inc/system.h"
#include "inc/irq.h"
#include "inc/system.cpp"
#include "inc/uart.cpp"
#include "inc/uartio.cpp"
//#include "inc/keyScreen.cpp"
#include "inc/print.cpp"


int randomTest(){
  int seed = getSeed();
  rndSetSeed(seed);
  
  unsigned int datas[1024];
  unsigned int addrs[1024];
  
  for(int i=0;i<1024;i++){
    unsigned int _rndv = (unsigned int)rnd();
    addrs[i] = _rndv % 0x2000000;
    _rndv = (unsigned int)rnd();
    datas[i] = _rndv;
  }

  for(int i=0;i<1024;i++){
    unsigned int* p = (unsigned int*)addrs[i];
    *p = datas[i];
  }

  int err = 0;
  for(int i=0;i<1024;i++){
    unsigned int* p = (unsigned int*)addrs[i];
    if(*p != datas[i]){
      err++;
    }
  }
  print("err cnt:");printInt(err);print("\r\n");
}

void readpageTest()
{
  int seed = getSeed();
  rndSetSeed(seed);

  unsigned int otherPage = 1;

  while ((otherPage >= 1 && otherPage <= 15))
  {
    unsigned int _rndv = (unsigned int)rnd();
    otherPage = _rndv % 0x2000000;
    otherPage = otherPage >> 10;
  }
  print("otherPage:");printHex((int)otherPage<<10);print("\r\n");

  unsigned int otherPageData[256];
  
  for(int i=0;i<256;i++){
    unsigned int _rndv = (unsigned int)rnd();
    otherPageData[i] = _rndv;
    unsigned int* p = (unsigned int*)((otherPage << 10)+(i<<2));
    *p = otherPageData[i];
  }

  int err = 0;
  for(int i=0;i<256;i++){
    unsigned int* p = (unsigned int*)((otherPage << 10)+(i<<2));
    if(*p != otherPageData[i]){
      err++;
    }
  }
  print("err cnt:");printInt(err);print("\r\n");
}


void writebackTest()
{
  int seed = getSeed();
  rndSetSeed(seed);

  unsigned int targetPage = 1;

  while (targetPage >= 1 && targetPage <= 15)
  {
    unsigned int _rndv = (unsigned int)rnd();
    targetPage = _rndv % 0x2000000;
    targetPage = targetPage >> 10;
  }

  unsigned int otherPage = 1;

  while ((otherPage >= 1 && otherPage <= 15) || otherPage == targetPage)
  {
    unsigned int _rndv = (unsigned int)rnd();
    otherPage = _rndv % 0x2000000;
    otherPage = otherPage >> 10;
  }
  print("targetPage:");printHex((int)targetPage<<10);print("\r\n");

  print("otherPage:");printHex((int)otherPage<<10);print("\r\n");

  unsigned int otherPageData[256];

  unsigned int data[256];

  for(int i=0;i<256;i++){
    unsigned int _rndv = (unsigned int)rnd();
    otherPageData[i] = _rndv;
    unsigned int* p = (unsigned int*)((otherPage << 10)+(i<<2));
    *p = otherPageData[i];
  }

  {
    unsigned int* p = (unsigned int*)((targetPage << 10)+0);
    volatile unsigned int temp = *p;
  }

  for(int i=0;i<256;i++){
    unsigned int _rndv = (unsigned int)rnd();
    data[i] = _rndv;
    unsigned int* p = (unsigned int*)((targetPage << 10)+(i<<2));
    *p = data[i];
  }

  for (int j = 0; j < 40; j++)
  {
    for (int i = 1; i <= 15; i++)
    {
      unsigned int temppage = (unsigned int)i;
      unsigned int* p = (unsigned int*)((temppage << 10)+0);
      volatile unsigned int temp = *p;
    }
  }
  
  print("pause1...\r\n");
  getc();

  int err = 0;
  for(int i=0;i<256;i++){
    unsigned int* p = (unsigned int*)((otherPage << 10)+(i<<2));
    if(*p != otherPageData[i]){
      err++;
    }
  }
  print("err cnt1:");printInt(err);print("\r\n");

  print("pause2...\r\n");
  getc();

  err = 0;
  for(int i=0;i<256;i++){
    unsigned int* p = (unsigned int*)((targetPage << 10)+(i<<2));
    if(*p != data[i]){
      err++;
    }
  }
  print("err cnt2:");printInt(err);print("\r\n");
}


void flushTest()
{
  int seed = getSeed();
  rndSetSeed(seed);

  unsigned int targetPage = 1;

  while (targetPage >= 0 && targetPage <= 15)
  {
    unsigned int _rndv = (unsigned int)rnd();
    targetPage = _rndv % 0x2000000;
    targetPage = targetPage >> 10;
  }
  print("targetPage:");printHex((int)targetPage<<10);print("\r\n");

  unsigned int data[256];

  {
    unsigned int* p = (unsigned int*)((targetPage << 10)+0);
    volatile unsigned int temp = *p;
  }

  for(int i=0;i<256;i++){
    unsigned int _rndv = (unsigned int)rnd();
    data[i] = _rndv;
    unsigned int* p = (unsigned int*)((targetPage << 10)+(i<<2));
    *p = data[i];
  }
  
  for (int i = 0; i <= 15; i++)
  {
    unsigned int temppage = (unsigned int)i;
    unsigned int* p = (unsigned int*)((temppage << 10)+0);
    volatile unsigned int temp = *p;
  }
  
  print("pause1...\r\n");
  getc();

  flushCache((void *)(targetPage << 10));
  print("pause2...\r\n");
  getc();

  for (int i = 0; i <= 15; i++)
  {
    unsigned int temppage = (unsigned int)i;
    unsigned int* p = (unsigned int*)((temppage << 10)+0);
    volatile unsigned int temp = *p;
  }

  int err = 0;
  for(int i=0;i<256;i++){
    unsigned int* p = (unsigned int*)((targetPage << 10)+(i<<2));
    if(*p != data[i]){
      err++;
    }
  }
  print("err cnt:");printInt(err);print("\r\n");


}

int main(){
  stdioInit(1024);
  print("mem test:\r\n");
  print("1:randomTest\r\n");
  print("2:readpageTest\r\n");
  print("3:writebackTest\r\n");
  print("4:flushTest\r\n");

  int val1 = getc();
  if(val1=='1'){
    randomTest();
  }
  if(val1=='2'){
    readpageTest();
  }
  if(val1=='3'){
    writebackTest();
  }
  if(val1=='4'){
    flushTest();
  }
  
  
  
  while(1);
}
