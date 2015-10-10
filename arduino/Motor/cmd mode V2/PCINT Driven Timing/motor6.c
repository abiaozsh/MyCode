#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1
#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)
#include "D:\MyCode\arduino\_Common\serial.h"

#define MaxPonTime 2000

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:OCR1A)

//drA3
#define drDAT (PINA & _BV(7)) /*DAT*/
#define drCLK (PINA & _BV(6)) /*DAT*/

#define LEDInit PORTB &= ~_BV(3);
#define LEDOn   DDRB |= _BV(3);
#define LEDOff  DDRB &= ~_BV(3);

//2 1 0
//5 4 3 2 1 0
#define PORT6O PORTA
#define DDR6O DDRA

#define PORT3I PORTB
#define DDR3I DDRB
#define PIN3I PINB

#define BP1D _BV(5)
#define BP1U _BV(4)
#define BP1A _BV(0)
#define BP2D _BV(3)
#define BP2U _BV(2)
#define BP2A _BV(1)
#define BP3D _BV(1)
#define BP3U _BV(0)
#define BP3A _BV(2)

#define StartRpm 8192

#define CmdNextStep Step = NextStep[TempStep];
uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};
//power on
#define CmdPWROn  PORT6O = PWR_ON[TempStep];
//power off
#define CmdPWROff PORT6O = PWR_OFF[TempStep];

//下臂常开
uint8_t PWR_ON[] = {
	BP1U + BP1D + BP3D, // 1-2
	BP1U + BP1D + BP2D, // 1-3
	BP2U + BP1D + BP2D, // 2-3
	BP2U + BP3D + BP2D, // 2-1
	BP3U + BP3D + BP2D, // 3-1
	BP3U + BP1D + BP3D  // 3-2
};
uint8_t PWR_OFF[] = {
	0    + BP1D + BP3D, // 1-2
	0    + BP1D + BP2D, // 1-3
	0    + BP1D + BP2D, // 2-3
	0    + BP3D + BP2D, // 2-1
	0    + BP3D + BP2D, // 3-1
	0    + BP1D + BP3D  // 3-2
};

volatile uint8_t GetDigitRead;
volatile uint8_t GetDigitReadBaseVal;

volatile uint8_t Step = 0;
uint8_t Status = 0;//0 halt ,1 running, 2 starting
uint8_t StartUpCount1=0;
volatile uint16_t TargetRPM=1000;//bit16 = start flg rest is data
volatile uint8_t FStart = 0;
volatile uint16_t rpm;
///volatile uint16_t LastRpm;
volatile uint16_t NextPower = 0;

uint16_t rpms[8];
uint8_t rpmsIdx = 0;

uint8_t CMD = 0;
uint8_t TempData=0;
uint8_t TempDataCnt=8;
//uint8_t OutData=0;
//volatile uint16_t rpmSend;

void ClockInit();
void TimerInit();
void PCIntInit();
void test();
void adj();

int main(void) {
  ClockInit();//初始化时钟：1MHz -> 8MHz
  TimerInit();//初始化定时器 1/8
  LEDInit;
  SerialInit();
  
  uint16_t time;
  for(;;) {
    //case0
    OCR1A = 10;rpm=1;
    TCNT1 = 0;
    test();
    time = TCNT1;SerialSend('0');SendInt(time);SerialSend('\r');SerialSend('\n');
    
    //case1
    OCR1A = 1;rpm=10;GetDigitRead=1;GetDigitReadBaseVal=0;
    TCNT1 = 0;
    test();
    time = TCNT1;SerialSend('1');SendInt(time);SerialSend('\r');SerialSend('\n');
    
    //case2
    OCR1A = 1;rpm=10;GetDigitRead=1;GetDigitReadBaseVal=1;FStart=1;
    TCNT1 = 0;
    test();
    time = TCNT1;SerialSend('2');SendInt(time);SerialSend('\r');SerialSend('\n');

    //case3
    OCR1A = 1;rpm=9000;GetDigitRead=1;GetDigitReadBaseVal=1;FStart=0;Status=1;
    TCNT1 = 0;
    test();
    time = TCNT1;SerialSend('3');SendInt(time);SerialSend('\r');SerialSend('\n');

    //case4
    OCR1A = 1;rpm=1000;GetDigitRead=1;GetDigitReadBaseVal=1;FStart=0;Status=1;
    rpms[0] = rpm;rpms[1] = rpm;rpms[2] = rpm;rpms[3] = rpm;rpms[4] = rpm;rpms[5] = rpm;rpms[6] = rpm;rpms[7] = rpm;
    TargetRPM = 2000;
    TCNT1 = 0;
    test();
    time = TCNT1;SerialSend('4');SendInt(time);SerialSend('\r');SerialSend('\n');

    //case5
    OCR1A = 1;rpm=5000;GetDigitRead=1;GetDigitReadBaseVal=1;FStart=0;Status=0;StartUpCount1=30;
    TCNT1 = 0;
    test();
    time = TCNT1;SerialSend('5');SendInt(time);SerialSend('\r');SerialSend('\n');
    
    }
}

void ClockInit() {
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  //CLKPR = 3;//1/8
  CLKPR = 0;//1/1 //8MHz
}

void TimerInit() {
  TCCR1A = 0;
  TCCR1B = 2;//  1/8	1MHz 1us
  TCCR1C = 0;
}


//过零事件
void test(){
  LEDOn;
  uint16_t temp = (rpm>>1);//?? >>2
  if(currTick>=temp)
  {
    uint8_t TempStep = Step;
    uint8_t valbase = GetDigitReadBaseVal;
    uint8_t val = GetDigitRead;
    //确认过零
    if(val!=valbase)
    {
      if(!FStart)
      {
        CmdPWROff;
        CmdNextStep;
///        LastRpm = rpm;
        //记录当前转速
        rpm = currTick;
        //换向前处理结束
        ////////////////////////////////////////////////////////////////////////////////////////////
        //换向开始点
        
        //定时器清零
        OCR1A = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
        uint16_t Power = NextPower;
        if(Power)
        {
          CmdPWROn;
        }
        else
        {
          CmdPWROff;
        }
        OCR1A = Power;
        OCR1A = TargetRPM;
        adj();
      }
    }
  }
  LEDOff;
}

void adj() {
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      StartUpCount1 = 0;
      Status = 0;//halt
    }
    else
    {
      rpms[rpmsIdx] = rpm;
      rpmsIdx++;
      rpmsIdx&=7;
      uint8_t i;
      uint16_t avgrpm=0;
      for(i=0;i<8;i++)
      {
        avgrpm+=rpms[i];
      }
      avgrpm>>=3;
      
      uint16_t tempPower = NextPower;
      uint16_t TempTargetRPM = TargetRPM;
      if(avgrpm>TempTargetRPM)//little bit slow
      {
        uint16_t diff = (avgrpm-TempTargetRPM)>>4;//2
        if(tempPower+diff>10000)
        {
          tempPower = 10000;
        }
        else
        {
          tempPower += diff;
        }
      }
      else//little bit fast
      {
        uint16_t diff = (TempTargetRPM-avgrpm)>>3;//
        if(tempPower<diff)
        {
          tempPower = 0;
        }
        else
        {
          tempPower -= diff;
        }
      }
      NextPower = tempPower;
    }
  }
  else
  {
    //TODO
    NextPower = 0;
    /// && (rpm>(LastRpm<<1)) && (rpm<(LastRpm>>1))
    uint16_t temprpm;
    temprpm = rpm;
    if(temprpm < StartRpm && temprpm > (StartRpm>>2))//fast enough but not too fast
    {
      StartUpCount1++;
      rpms[rpmsIdx] = temprpm;
      rpmsIdx++;
      rpmsIdx&=7;
    }
    else
    {
      StartUpCount1 = 0;
    }
    if(StartUpCount1>20)
    {
      Status = 1;
      NextPower = 1000;
    }
  }
}




