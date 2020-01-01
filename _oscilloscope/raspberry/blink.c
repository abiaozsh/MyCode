#include <stdio.h>

#include "wiringPi.h"
#include "wiringPi.c"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  21, 29
#define  LED  12

#define  REQ  16
#define  ACK  20
#define  DAT  21


//  BCM  GPIO
//   0,  30
//   1,  31
//   2,   8
//   3,   9
//   4,   7
//   5,  21
//   6,  22
//   7,  11
//   8,  10
//   9,  13
//  10,  12
//  11,  14
//  12,  26
//  13,  23
//  14,  15
//  15,  16
//  16,  27
//  17,   0
//  18,   1
//  19,  24
//  20,  28
//  21,  29
//  22,   3
//  23,   4
//  24,   5
//  25,   6
//  26,  25
//  27,   2

//  28, 
//  29, 
//  30, 
//  31, 

int main (void)
{
  wiringPiSetup();
  
  //pinMode(LED, OUTPUT);
  
  pinMode(REQ, OUTPUT);
  pinMode(ACK, INPUT);
  pinMode(DAT, INPUT);

  digitalWrite(REQ, LOW);


  char buff[1048576];

  FILE *fp = NULL;
  fp = fopen("/mnt/tmpfs/a.bin", "wb+");

  
  for(int i=0;i<1048576;i++){
    digitalWrite(REQ, HIGH);
    while(digitalRead(ACK)==LOW);
    buff[i] = (char)digitalRead(DAT);
    digitalWrite(REQ, LOW);
    while(digitalRead(ACK)==HIGH);
  }
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
