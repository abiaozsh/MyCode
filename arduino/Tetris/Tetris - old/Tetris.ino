#include <LCD5110_Basic.h>
#define _SCK  5
#define _MOSI 6
#define _DC 7
#define _CS 8
#define _RST 9

LCD5110 myGLCD(_SCK,_MOSI,_DC,_RST,_CS);

#define KEY_LEFT      11
#define KEY_RIGHT     10
#define KEY_DOWN      A1
#define KEY_DOWNFAST  A0
#define KEY_ROTATE    12

byte buff[504];
byte Board[200];
byte PosX = 0;
byte PosY = 0;
byte NextShapeNum = 0;
byte NowShapeNum = 0;
byte NowShapeRotateIdx = 0;

int state_LEFT = 0;
int state_RIGHT = 0;
int state_DOWN = 0;
int state_DOWNFAST = 0;
int state_ROTATE = 0;
unsigned long NextSecond = 0;

void setup() {
  myGLCD.InitLCD();
  InitImg();
  DrawInitScreen();
  while(true)
  {
    if(digitalRead(KEY_ROTATE)!=1)
    {
      break;
    }
  }
  
  randomSeed(micros());

  Init();

  NextSecond = millis()+1000;
}

void loop() {
  while(true)
  {
    if(digitalRead(KEY_LEFT)!=state_LEFT)
    {
      state_LEFT=digitalRead(KEY_LEFT);
      if(state_LEFT==0)
      {
        Move(1);
        break;
      }
    }
    if(digitalRead(KEY_RIGHT)!=state_RIGHT)
    {
      state_RIGHT=digitalRead(KEY_RIGHT);
      if(state_RIGHT==0)
      {
        Move(255);
        break;
      }
    }
    if(digitalRead(KEY_DOWN)!=state_DOWN)
    {
      state_DOWN=digitalRead(KEY_DOWN);
      if(state_DOWN==0)
      {
        slowdown();
        break;
      }
    }
    if(digitalRead(KEY_DOWNFAST)!=state_DOWNFAST)
    {
      state_DOWNFAST=digitalRead(KEY_DOWNFAST);
      if(state_DOWNFAST==0)
      {
        down();
        break;
      }
    }
    if(digitalRead(KEY_ROTATE)!=state_ROTATE)
    {
      state_ROTATE=digitalRead(KEY_ROTATE);
      if(state_ROTATE==0)
      {
        rotate();
        break;
      }
    }
    if(millis()>NextSecond)
    {
      NextSecond = millis()+1000;
      timing();
      break;
    }
  }

  DrawBoard();
  myGLCD.drawBitmapBuff(0, 0, buff, 84, 48);
}




