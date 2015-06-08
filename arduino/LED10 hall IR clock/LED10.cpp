#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "LED10.h"
#include "data.h"
#include "IRlib.h"
#include "LEDlib.h"
#include "time.h"
#include "ir.h"


//D,C,E,B,F
//A0 - PF7


//byte data_line[30];

void loop() {
  int idx = 0;
  for(;;)
  {
    while((PINF&_BV(7))!=0){}
    ShowROM(idx*7,idx*7+7,data_num);
    /*
    if(idx>= 0&&idx<10)
    {
      ShowROM(cnt_p1,data_p1);
    }
    if(idx>=10&&idx<20)
    {
      ShowROM(cnt_p3,data_p3);
    }
    if(idx>=20&&idx<30)
    {
      ShowROM(cnt_p2,data_p2);
    }
    if(idx>=30&&idx<40)
    {
      ShowROM(cnt_p3,data_p3);
    }*/
    
    //ShowRAM(1,data_line);
    
    
    idx++;
    if(idx==10)
    {
      idx=0;
    }
  }
}

int main(void) {
  
  //Serial.begin(9600);
  InitIR();
  while(true)
  {
    loopir();
  }


  cli();
  initArray();
  rstPin();
  Timer0Init();
  Timer1Init();
  Timer3Init();
  initDDR();


  loop();
}

