#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define MaxPonTime 2000

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

//drA3
#define drDAT (PINA & _BV(7)) /*DAT*/
#define drCLK (PINA & _BV(6)) /*DAT*/

#define CPUOn   ;/*DDRB |= _BV(3) */
#define CPUOff  ;/*DDRB &= ~_BV(3)*/
#define STAOn   ;/*DDRB |= _BV(3) */
#define STAOff  ;/*DDRB &= ~_BV(3)*/
#define PWROn   DDRB |= _BV(3) ;/**/
#define PWROff  DDRB &= ~_BV(3);/**/
#define RPMFlip ;/*DDRB ^= _BV(3)*/

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

//a6 start signal / restartup signal
//a7 throttle

#define StartRpm 8192

#define CmdNextStep Step = NextStep[TempStep];RPMFlip;
uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};
//power on
#define CmdPWROn  PORT6O = PWR_ON[TempStep];PWROn;
//power off
#define CmdPWROff PORT6O = PWR_OFF[TempStep];PWROff;
//power off
#define CmdPWRDown PORT6O = 0;PWROff;

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

#define GetDigitRead (PIN3I & DigitRead[TempStep])
uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
#define GetDigitReadBaseVal (DigitReadBaseVal[TempStep])
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

volatile uint8_t Step = 0;
uint8_t Status = 0;//0 halt ,1 running
uint8_t StartUpCount1=0;
volatile uint16_t TargetRPM=0;//bit16 = start flg rest is data
volatile uint8_t FStart = 0;
volatile uint16_t rpm;
volatile uint16_t NextPower = 0;
uint32_t AccuPower = 0;//volatile uint16_t AccuPower = 0;
volatile uint8_t StartPower = 128;

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
void adj();

int main(void) {
  ClockInit();//初始化时钟：1MHz -> 8MHz
  TimerInit();//初始化定时器 1/8
  PCIntInit();//初始化模拟输入

  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  //初始化输入端口
  DDR3I = 0;
  PORT3I = 0;
  //主循环
  sei();
  for(;;) {
    asm volatile("sleep");
  }
}

void ClockInit() {
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
}

void TimerInit() {
  TCCR1A = 0;
  TCCR1B = 2;//  1/8	1MHz 1us
  TCCR1C = 0;
  TIMSK1 = 0;
  TIMSK1 |= _BV(OCIE1A);
}

void PCIntInit() {
  GIMSK |= _BV(PCIE0);
  GIMSK |= _BV(PCIE1);
  PCMSK1 |= _BV(0);
  PCMSK1 |= _BV(1);
  PCMSK1 |= _BV(2);
  PCMSK0 |= _BV(6);//CLK
}

//过零事件
ISR(PCINT1_vect){
  CPUOn;
  uint16_t temp;
  uint16_t tempRpm = rpm;//过零空窗期不检测
  
  if(tempRpm<1024)
  {
    temp = (tempRpm>>1);//?? >>2
  }
  else if(tempRpm<2048)
  {
    temp = (tempRpm>>2);//?? >>2
  }
  else if(tempRpm<4096)
  {
    temp = (tempRpm>>3);//?? >>2
  }
  else
  {
    temp = (tempRpm>>4);//?? >>2
  }

  if(currTick>=temp)
  {
    uint8_t TempStep = Step;
    uint8_t valbase = GetDigitReadBaseVal;
    uint8_t val = GetDigitRead;
    //确认过零
    if(val!=valbase)
    {
      CmdPWROff;
      CmdNextStep;
      //记录当前转速
      rpm = currTick;
      //换向前处理结束
      ////////////////////////////////////////////////////////////////////////////////////////////
      //换向开始点
      TempStep = Step;
      //定时器清零
      TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
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
      OCR1B = TargetRPM;
      adj();
    }
  }
  CPUOff;
}

void adj() {
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      StartUpCount1 = 0;
      Status = 0;//halt
      STAOff;
    }
    else
    {
      rpms[rpmsIdx] = rpm;
      uint16_t avgrpm=0;
      uint8_t i = rpmsIdx;
      rpmsIdx++;
      rpmsIdx&=7;
      avgrpm+=rpms[i--];i&=7;//0 8
      avgrpm+=rpms[i--];i&=7;//1 8
      rpms[i]>>=1;
      avgrpm+=rpms[i--];i&=7;//2 4
      avgrpm+=rpms[i--];i&=7;//3 4
      rpms[i]>>=1;
      avgrpm+=rpms[i--];i&=7;//4 2
      avgrpm+=rpms[i--];i&=7;//5 2
      avgrpm+=rpms[i--];i&=7;//6 2
      avgrpm+=rpms[i--];i&=7;//7 2
      avgrpm>>=2;
      uint16_t tempPower;
      uint16_t TempTargetRPM = TargetRPM;

      if(avgrpm>TempTargetRPM)//little bit slow
      {
        uint16_t diff = (avgrpm-TempTargetRPM);//2
        if(AccuPower+diff>2048000)
        {
          AccuPower = 2048000;
        }
        else
        {
          AccuPower += diff;
        }
        tempPower = ((diff)<<2)+(AccuPower>>8);
      }
      else//little bit fast
      {
        uint16_t diff = (TempTargetRPM-avgrpm);//
        if(AccuPower<diff)
        {
          AccuPower = 0;
        }
        else
        {
          AccuPower -= diff;
        }
        tempPower = AccuPower>>8;
      }

      NextPower = tempPower;
    }
  }
  else
  {
    if(FStart)
    {
      uint16_t temprpm;
      temprpm = rpm;
      NextPower = ((uint32_t)rpm * StartPower)>>8;
      if(temprpm < StartRpm && temprpm > (StartRpm>>3))//fast enough but not too fast
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
      if(StartUpCount1>120)
      {
        Status = 1;
        STAOn;
      }
      FStart--;
    }
    else
    {
      NextPower = 0;
      uint16_t temprpm;
      temprpm = rpm;
      if(temprpm < StartRpm && temprpm > (StartRpm>>3))//fast enough but not too fast
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
        STAOn;
        NextPower = ((uint32_t)rpm * StartPower)>>8;
      }
    }
  }
}

ISR(TIM1_COMPA_vect){
  uint8_t TempStep = Step;
  CmdPWROff;
}

#define CMD_SENDDATA1Xa   10  /*0~255       1x */
#define CMD_SENDDATA1Xb   11  /*256~511     1x */
#define CMD_SENDDATA2X    12  /*512~1023    2x */
#define CMD_SENDDATA4X    13  /*1024~2047   4x */
#define CMD_SENDDATA8X    14  /*2048~4095   8x */
#define CMD_SENDDATA16X   15  /*4096~8191  16x */
#define CMD_SENDDATA32X   16  /*8192~16383 32x */
#define CMD_START         20  /*START          */
#define CMD_STOP          25  /*STOP           */
#define CMD_SETSTARTPWR   30  /*set start power*/
#define CMD_LINEUP        40  /*LINEUP         */

ISR(PCINT0_vect){//先送高，后送低
  CPUOn;
  if(drCLK)//上升沿读取
  {
    uint8_t tempdrDAT = drDAT;
    if(Status)
    {
      sei();
    }
    if(TempDataCnt==8)
    {
      if(tempdrDAT)//起始位
      {
        TempData = 0;
        TempDataCnt = 0;
      }
    }
    else
    {
      TempData<<=1;
      if(tempdrDAT)
      {
        TempData |= 1;
      }
      TempDataCnt++;
      if(TempDataCnt==8)
      {
        if(CMD==0)
        {
          CMD = TempData;
        }
        else
        {
          switch(CMD)
          {
            case CMD_SENDDATA1Xa://   10  /*0~255       1x*/
              TargetRPM = TempData;
              break;
            case CMD_SENDDATA1Xb://   11  /*256~511     1x*/
              TargetRPM = TempData + 256;
              break;
            case CMD_SENDDATA2X://    12  /*512~1023    2x*/
              TargetRPM = (TempData<<1) + 512;
              break;
            case CMD_SENDDATA4X://    13  /*1024~2047   4x*/
              TargetRPM = (TempData<<2) + 1024;
              break;
            case CMD_SENDDATA8X://    14  /*2048~4095   8x*/
              TargetRPM = (TempData<<3) + 2048;
              break;
            case CMD_SENDDATA16X://   15  /*4096~8191  16x*/
              TargetRPM = (TempData<<4) + 4096;
              break;
            case CMD_SENDDATA32X://   16  /*8192~16383 32x*/
              TargetRPM = (TempData<<5) + 8192;
              break;
            case CMD_START:
            {
              uint8_t TempStep = Step;
              CmdNextStep;
              CmdPWROn;
              FStart = TempData;
              GIMSK |= _BV(PCIE1);
              break;
            }
            case CMD_STOP:
            {
              uint8_t TempStep = Step;
              GIMSK &= ~_BV(PCIE1);
              CmdPWRDown;
              FStart = 0;
              Status = 0;
              break;
            }
            case CMD_SETSTARTPWR:
              StartPower = TempData;
              break;
            case CMD_LINEUP:
            {
              uint8_t TempStep = Step;
              GIMSK &= ~_BV(PCIE1);
              uint8_t i;
              uint16_t j;
              for(i=TempData;i>0;i--)
              {
                CmdPWROn;
                for(j=0;j<i;j++)
                {
                  asm volatile("nop");
                }
                CmdPWRDown;
                for(j=0;j<1000;j++)
                {
                  asm volatile("nop");
                }
              }
              CmdPWROff;
              break;
            }
          }
          CMD = 0;
        }
      }
    }
  }
  CPUOff;
}
