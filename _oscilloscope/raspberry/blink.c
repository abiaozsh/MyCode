#include <stdio.h>

#include "wiringPi.h"
#include "wiringPi.c"
//wiringSerial.c
//wiringShift.c
//piHiPri.c
//piThread.c
//wiringPiSPI.c
//wiringPiI2C.c
//softPwm.c
//softTone.c
//mcp23008.c
//mcp23016.c
//mcp23017.c
//mcp23s08.c
//mcp23s17.c
//sr595.c
//pcf8574.c
//pcf8591.c
//mcp3002.c
//mcp3004.c
//mcp4802.c
//mcp3422.c
//max31855.c
//max5322.c
//ads1115.c
//sn3218.c
//bmp180.c
//htu21d.c
//ds18b20.c
//rht03.c
//drcSerial.c
//drcNet.c
//pseudoPins.c
//wpiExtensions.c


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	LED	29

int main (void)
{
   FILE *fp = NULL;
   fp = fopen("/mnt/tmpfs/a.bin", "wb+");

   char buff[1024];
   buff[0] = 0x55;
   
   for(int i=0;i<32768;i++){
	fwrite(&buff,sizeof(char),1024,fp);
   }
   //size_t fwrite(const void *ptr, size_t size_of_elements, size_t number_of_elements, FILE *a_file);

   //printf(wiringPiMode);
   
  wiringPiSetup();
  int aa = 11;
  printf("[%d]",aa);
  
  pinMode(LED, OUTPUT);
  
  //mypinMode(LED, OUTPUT);
  
  for (int i=0;i<10;i++)
  {
    digitalWrite (LED, HIGH) ;	// On
    delay (50) ;		// mS
    digitalWrite (LED, LOW) ;	// Off
    delay (50) ;
  }
  pinMode(LED, INPUT);
  fclose(fp);
  
  return 0 ;
}
