#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//drA3
#define drDAT (PINA & _BV(7)) /*DAT*/
#define drCLK (PINA & _BV(6)) /*DAT*/

#define CPUFree ;/*DDRB |= _BV(3) */ //free亮灯
#define CPUBusy ;/*DDRB &= ~_BV(3)*/ //busy暗灯
#define STAOn   DDRB |= _BV(3) ;/**/
#define STAOff  DDRB &= ~_BV(3);/**/
#define FSTOn   ;/*DDRB |= _BV(3) */
#define FSTOff  ;/*DDRB &= ~_BV(3)*/
#define PWROn   ;/*DDRB |= _BV(3) */
#define PWROff  ;/*DDRB &= ~_BV(3)*/
#define RPMFlip ;/*DDRB ^= _BV(3)*/

#define StartRpm 8192

uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};

uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

volatile uint8_t Step = 0;
volatile uint8_t FStart = 0;
volatile uint8_t MaxPower = 0;
volatile uint8_t Pitch = 1;
volatile uint8_t noskip = 1;
volatile uint8_t Status = 0;//0 halt ,1 running, 2 starting

uint16_t rpm;
uint16_t NextPower = 0;

uint8_t CMD = 0;
uint8_t TempData=0;
uint8_t TempDataCnt=8;

void adj();

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  TIMSK1 |= _BV(OCIE1A);
  //初始化输入
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT6);//CLK

  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();
  //主循环
  for(;;) 
  {
    //定时器清零
    TCNT1 = 0;
    uint16_t Power = NextPower;
    uint8_t tempStep = Step;
    if(Power)
    {
      PORT6O = PWR_ON[tempStep];PWROn;//CmdPWROn;
    }
    else
    {
      PORT6O = PWR_OFF[tempStep];PWROff;//CmdPWROff;
    }
    OCR1A = Power;
    //转速调整
    adj();
    RPMFlip;
    //等待过零
    {
      uint8_t valbase = DigitReadBaseVal[tempStep];
      uint8_t drMask = DigitRead[tempStep];
      
      uint16_t temp = (rpm>>1);
      CPUFree;
      while(TCNT1<temp);
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
      CPUBusy;
    }
    if(Pitch && !FStart)//
    {
      uint16_t tmp = (rpm>>3)+(rpm>>2)+TCNT1;
      CPUFree;
      while(TCNT1<tmp);
      CPUBusy;
    }
    PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
    Step = NextStep[Step];//CmdNextStep;
    //记录当前转速
    rpm = TCNT1;
  }
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
}

uint16_t _MaxPower(uint16_t val){
  uint32_t temp = val;
  //28 耗时
  //224clock
  temp*=MaxPower;
  temp>>=8;
  return (uint16_t)temp;
}

void adj(){
  NextPower = 0;
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      Status = 0;STAOff;
    }
    else
    {
      NextPower = _MaxPower(rpm);
    }
  }
  else
  {
    if(FStart)
    {
      NextPower = _MaxPower(rpm);
    }
	}
}

#define CMD_SENDDATA1Xa   1  /*0~255       1x */
#define CMD_SENDDATA1Xb   2  /*256~511     1x */
#define CMD_SENDDATA2X    3  /*512~1023    2x */
#define CMD_SENDDATA4X    4  /*1024~2047   4x */
#define CMD_SENDDATA8X    5  /*2048~4095   8x */
#define CMD_SENDDATA16X   6  /*4096~8191  16x */
#define CMD_START         7  /*START          */
#define CMD_SETMAXPWR     8  /*set max power  */
#define CMD_SAVESET       9  /*savesetting    */
#define CMD_PITCH         10  /*PITCH         */
#define CMD_REVERSE       11  /*REVERSE       */
#define CMD_SETCPU        12  /*SETCPU        */
#define CMD_NOSTART       13  /*START off     */
#define CMD_SETPWRSIMP    14  /*set power simple */

ISR(PCINT0_vect){//先送高，后送低
  if(TempDataCnt == 8)
  {
    if(drDAT)//起始位
    {
      TempData = 0;
      TempDataCnt = 0;
    }
  }
  else
  {
    TempData <<= 1;
    if(drDAT)
    {
      TempData |= 1;
    }
    TempDataCnt++;
    if(TempDataCnt == 8)
    {
      if(CMD == 0)
      {
        CMD = TempData;
      }
      else
      {
        switch(CMD)
        {
          case CMD_SENDDATA1Xa:break;
          case CMD_SENDDATA1Xb:break;
          case CMD_SENDDATA2X:break;
          case CMD_SENDDATA4X:break;
          case CMD_SENDDATA8X:break;
          case CMD_SENDDATA16X:break;
          case CMD_START:
          {
            FStart = 1;FSTOn;
            noskip = 0;
            break;
          }
          case CMD_NOSTART:
          {
            FStart = 0;FSTOff;
            Status = 1;STAOn;
            break;
          }
          case CMD_SETMAXPWR:break;
          case CMD_SAVESET:break;
          case CMD_PITCH:
            Pitch = TempData;
            break;
          case CMD_REVERSE:break;
          case CMD_SETCPU:break;
          case CMD_SETPWRSIMP:
            MaxPower = TempData;
            break;
        }
        CMD = 0;
      }
    }
  }
}
