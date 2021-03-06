#include <stdlib.h>
#include <stdio.h>
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

/*
sudo mount tmpfs /mnt/tmpfs -t tmpfs -o size=50m
cd /mnt/tmpfs
gcc -Wall ./test.c -o ./test
./test
*/

void setCmd(int cmd){
  digitalWrite(CMD0, (cmd & 0x01)?HIGH:LOW);
  digitalWrite(CMD1, (cmd & 0x02)?HIGH:LOW);
  digitalWrite(CMD2, (cmd & 0x04)?HIGH:LOW);
  digitalWrite(CMD3, (cmd & 0x08)?HIGH:LOW);
  delay(1);
}

void req(){
  digitalWrite(REQ, HIGH);
  while(digitalRead(ACK)==LOW);
  digitalWrite(REQ, LOW);
  while(digitalRead(ACK)==HIGH);
}


int main (void)
{
  
  volatile unsigned int* gpio = wiringPiSetup();
  volatile unsigned int* gpio13 = gpio + 13;
  volatile unsigned int* gpio10 = gpio + 10;
  volatile unsigned int* gpio7 = gpio + 7;
  
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
  delay(1);
  digitalWrite(RST, LOW);
  //init ///////////////////////////////////
  
  setCmd(0x00);
  digitalWrite(REQ, HIGH);
  delay(1);
  printf("%d",digitalRead(ACK));
  return 0;

  
  setCmd(0);//reset address
  req();

  setCmd(1);//read
  int page = 16;
  int pagesize = 1024*1024;
  
  short* buff;
  //[page*pagesize]
  buff = (short*)malloc(sizeof(short) * page * pagesize);
   
  for(int j=0;j<page;j++){
    for(int i=0;i<pagesize;i++){
      REQHIGH;//digitalWrite(REQ, HIGH);
      while(!PINACK);// digitalRead(ACK)==LOW
      short data = read16();
      buff[j*pagesize+i]=data;
      REQLOW;//digitalWrite(REQ, LOW);
      while(PINACK);//digitalRead(ACK)==HIGH
    }
    printf("%d\r\n",j);
  }
  //for(int i=0;i<2048;i++){
  //  printf("%d\r\n",buff[i]);
  //}
  //for(int j=0;j<128;j++){
  //  for(int i=0;i<16;i++){
  //    printf("%04x ",((int)buff[j*16+i])&0x0000FFFF);
  //  }
  //  printf("\r\n");
  //}
  
  FILE *fp = NULL;
  fp = fopen("/mnt/tmpfs/a.bin", "wb+");
  
  fwrite(buff,sizeof(short),page * pagesize,fp);
  
  fclose(fp);

  
  
  
  free(buff);
  
  for(int i=0;i<32;i++){
    pinMode(i, INPUT);
  }
  return 0;
}
