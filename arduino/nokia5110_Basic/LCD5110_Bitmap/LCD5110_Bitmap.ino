// LCD5110_Bitmap (C)2013 Henning Karlsen
// web: http://www.henningkarlsen.com/electronics
//
// This program is a demo of how to use bitmaps.
// You can also see how to use invert().
//
// This program requires a Nokia 5110 LCD module.
//
// It is assumed that the LCD module is connected to
// the following pins using a levelshifter to get the
// correct voltage to the module.
//      SCK  - Pin 8
//      MOSI - Pin 9
//      DC   - Pin 10
//      RST  - Pin 11
//      CS   - Pin 12
//
#include <LCD5110_Basic.h>

#define _SCK  5
#define _MOSI 6
#define _DC 7
#define _CS 8
#define _RST 9

//LCD5110 myGLCD(8,9,10,11,12);
LCD5110 myGLCD(_SCK,_MOSI,_DC,_RST,_CS);

extern uint8_t arduino_logo[];
extern uint8_t oshw_logo[];

void setup()
{
  myGLCD.InitLCD();
  Serial.begin(9600);
}

void loop()
{
  
  unsigned long CurrentTime = micros();
  myGLCD.drawBitmap(0, 0, arduino_logo, 84, 48);
  Serial.print("1:");
  Serial.println(micros()-CurrentTime);
  delay(200);
  
  CurrentTime = micros();
  myGLCD.clrScr();
  Serial.print("2:");
  Serial.println(micros()-CurrentTime);
  delay(200);
  
  CurrentTime = micros();
  myGLCD.drawBitmap(14, 0, oshw_logo, 56, 48);
  Serial.print("3:");
  Serial.println(micros()-CurrentTime);
  delay(200);
}
