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

/*
sudo mount tmpfs /mnt/tmpfs -t tmpfs -o size=300m
cd /mnt/tmpfs
gcc -Wall ./blink.c -o ./blink
./blink
*/
int main (void)
{
  wiringPiSetup();
  
  for(int i=0;i<32;i++){
    pinMode(i, INPUT);
  }
  pinMode(REQ, OUTPUT);
  pinMode(RST, OUTPUT);

  digitalWrite(REQ, LOW);
  digitalWrite(RST, HIGH);
  delay(1);
  digitalWrite(RST, LOW);
  for(int j=0;j<100;j++){
  for(int i=0;i<1024*1024;i++){
    digitalWrite(REQ, HIGH);
    while(digitalRead(ACK)==LOW);
    short data = read16();
    if(data!=(short)i){
      printf("%d\r\n",i);
      return 0;
    }
    digitalWrite(REQ, LOW);
    while(digitalRead(ACK)==HIGH);

  }
  printf("%d\r\n",j);
  }
  return 0;
  char buff[1048576];

  FILE *fp = NULL;
  fp = fopen("/mnt/tmpfs/a.bin", "wb+");

  
  //size_t fwrite(const void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);
  fwrite(&buff,sizeof(char),1048576,fp);
  fclose(fp);

  
  //for (int i=0;i<10;i++)
  //{
  //  digitalWrite (LED, HIGH) ;	// On
  //  delay (50) ;		// mS
  //  digitalWrite (LED, LOW) ;	// Off
  //  delay (50) ;
  //}
  //
  //pinMode(LED, INPUT);
  pinMode(REQ, INPUT);
  
  return 0;
}
