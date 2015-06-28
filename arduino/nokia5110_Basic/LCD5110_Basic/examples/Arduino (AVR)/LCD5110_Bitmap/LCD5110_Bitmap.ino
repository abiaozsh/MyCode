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

LCD5110 myGLCD(10,11,12,A0,13);

extern uint8_t arduino_logo[];
extern uint8_t oshw_logo[];

void setup()
{
  myGLCD.InitLCD();
}

void loop()
{
  myGLCD.drawBitmap(0, 0, arduino_logo, 84, 48);
  delay(4000);
  for (int i=0; i<2; i++)
  {
    myGLCD.invert(true);
    delay(500);
    myGLCD.invert(false);
    delay(500);
  }
  delay(4000);

  myGLCD.clrScr();
  myGLCD.drawBitmap(14, 0, oshw_logo, 56, 48);
  delay(4000);
  for (int i=0; i<2; i++)
  {
    myGLCD.invert(true);
    delay(500);
    myGLCD.invert(false);
    delay(500);
  }
  delay(4000);
}


void drawBitmap2(int x, int y, bitmapdatatype bitmap, int sx, int sy)
{
  int starty, rows;

  //starty = y / 8;
  starty = y >> 3;

  //if (sy%8==0)
  if (sy&7==0)
    //rows=sy/8;  
    rows=sy>>3;  
  else
    //rows=(sy/8)+1;
    rows=(sy>>3)+1;

  for (int cy=0; cy<rows; cy++)
  {
    myGLCD._LCD_Write(PCD8544_SETYADDR | (starty+cy), LCD_COMMAND);
    myGLCD._LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);
    int idx = cy*sx;
    for(int cx=0; cx<sx; cx++)
      myGLCD._LCD_Write(bitmapbyte(idx+cx), LCD_DATA);
  }      
  myGLCD._LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  myGLCD._LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);

}



