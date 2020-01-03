#include <stdio.h>
#include "wiringPi.h"
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

/*
sudo mount tmpfs /mnt/tmpfs -t tmpfs -o size=300m
cd /mnt/tmpfs
gcc -Wall ./blink.c -o ./blink
./blink
*/

//0000
void cmdClearAddress(){
  digitalWrite(CMD0, LOW);
  digitalWrite(CMD1, LOW);
  digitalWrite(CMD2, LOW);
  digitalWrite(CMD3, LOW);
  delay(1);
}
void req(){
  digitalWrite(REQ, HIGH);
  while(digitalRead(ACK)==LOW);
  digitalWrite(REQ, LOW);
  while(digitalRead(ACK)==HIGH);
}
//0001
void cmdRead(){
  digitalWrite(CMD0, HIGH);
  digitalWrite(CMD1, LOW);
  digitalWrite(CMD2, LOW);
  digitalWrite(CMD3, LOW);
  delay(1);
}


int main (void)
{
  wiringPiSetup();
  
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
  
  cmdClearAddress();
  req();

  cmdRead();
  
  short buff[2048];
  for(int i=0;i<2048;i++){
    digitalWrite(REQ, HIGH);
    while(digitalRead(ACK)==LOW);
    short data = read16();
    buff[i]=data;
    digitalWrite(REQ, LOW);
    while(digitalRead(ACK)==HIGH);
  }
  //for(int i=0;i<2048;i++){
  //  printf("%d\r\n",buff[i]);
  //}
  for(int j=0;j<128;j++){
    for(int i=0;i<16;i++){
      printf("%04x ",((int)buff[j*16+i])&0x0000FFFF);
    }
    printf("\r\n");
  }
  
  return 0;
//  char buff[1048576];
//
//  FILE *fp = NULL;
//  fp = fopen("/mnt/tmpfs/a.bin", "wb+");
//
//  
//  //size_t fwrite(const void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);
//  fwrite(&buff,sizeof(char),1048576,fp);
//  fclose(fp);
//
//  
//  //for (int i=0;i<10;i++)
//  //{
//  //  digitalWrite (LED, HIGH) ;	// On
//  //  delay (50) ;		// mS
//  //  digitalWrite (LED, LOW) ;	// Off
//  //  delay (50) ;
//  //}
//  //
//  //pinMode(LED, INPUT);
//  pinMode(REQ, INPUT);
//  
  return 0;
}
