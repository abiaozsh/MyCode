#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define MaxPonTime 2000

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

//drA3
#define drDAT (PINA & _BV(7)) /*DAT*/
#define drCLK (PINA & _BV(6)) /*DAT*/

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

#define StartRpm 15000

#define CmdNextStep Step = NextStep[Step];
uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};
//power on
#define CmdPWROn  PORT6O = PWR_ON[Step];
//power off
#define CmdPWROff PORT6O = PWR_OFF[Step];

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

#define GetDigitRead (PIN3I & DigitRead[Step])
uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
#define GetDigitReadBaseVal (DigitReadBaseVal[Step])
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

volatile uint8_t Step = 0;
volatile   uint8_t Status = 0;//0 halt ,1 running, 2 starting
volatile   uint8_t StartUpCount1=0;
volatile uint16_t TargetRPM=1000;//bit16 = start flg rest is data
volatile uint8_t FStart = 0;
volatile uint16_t rpm;
volatile   uint16_t startupCurrent;
volatile   uint16_t Power = 0;
volatile   uint16_t NextPower = 0;

volatile   uint8_t CMD = 0;
volatile   uint8_t TempData=0;
volatile   uint8_t OutData=0;
volatile   uint8_t TempDataCnt=8;
volatile   uint8_t DataReceiveCnt=0;
volatile   uint16_t TargetRPMBuff;
volatile   uint16_t rpmSend;

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
  //CLKPR = 3;//1/8
  CLKPR = 0;//1/1 //8MHz
}

void TimerInit() {
  TCCR1A = 0;
  TCCR1B = 2;//  1/8	1MHz 1us
  TCCR1C = 0;
  TIMSK1 = 0;
  TIMSK1 |= _BV(OCIE1B);
  TIMSK1 |= _BV(OCIE1A);
  //OCR1A 关断定时器=power
  //OCR1B 强制启动定时器=rpm
  //TIMSK1 |= _BV(TOIE1);
}

void PCIntInit() {
  GIMSK |= _BV(PCIE0);
  GIMSK |= _BV(PCIE1);
  PCMSK1 |= _BV(0);
  PCMSK1 |= _BV(1);
  PCMSK1 |= _BV(2);
  PCMSK0 |= _BV(6);//CLK
}

void adj() {
  NextPower=0;
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      StartUpCount1 = 0;
      Status = 0;//halt
    }
    else
    {
      if(rpm>TargetRPM)//bit slow && running
      {
        NextPower = 10000;
        //TODO PID
        //if(rpm>200)//低转速下进行精细运算
        //{
        //	uint32_t temp = TargetRPM;
        //	temp = temp << 16;//*65536
        //	temp = temp / rpm;
        //	temp = 65536 - temp;//65536~~>0 (0 为转速非常接近)
        //	temp = rpm * temp;
        //	temp = temp >> 15;// 数字越大越柔和    /1024
        //	NextPower = temp;
        //}
        //else
        //{
        //	NextPower = 1000;
        //}
      }
      else
      {
        NextPower = 0;
      }
    }
  }
  else
  {
    if(rpm<StartRpm&&rpm>(StartRpm>>3))//fast enough but not too fast
    {
      StartUpCount1++;
    }
    else
    {
      StartUpCount1 = 0;
    }
    if(StartUpCount1>40)
    {
      Status = 1;
    }
  }
}

//1正常运转状态
//2低速切断状态/高频抖动状态
//手动启动状态
//3强制启动状态

//过零事件
ISR(PCINT1_vect){
  uint16_t temp = (rpm>>1);//?? >>2
  if(currTick>=temp)
  {
    uint8_t valbase = GetDigitReadBaseVal;
    uint8_t val = GetDigitRead;
    //确认过零
    if(val!=valbase)
    {
      if(!FStart)
      {
        CmdPWROff;
        CmdNextStep;
        //记录当前转速
        rpm = currTick;
        //换向前处理结束
        ////////////////////////////////////////////////////////////////////////////////////////////
        //换向开始点
        
        //定时器清零
        TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
        Power = NextPower;
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
  }
}
ISR(TIM1_COMPA_vect){
  CmdPWROff;
}

ISR(TIM1_COMPB_vect){
  if(FStart)//强制换向
  {
    CmdPWROff;
    CmdNextStep;
    //记录当前转速
    rpm = currTick;
    //换向前处理结束
    ////////////////////////////////////////////////////////////////////////////////////////////
    //换向开始点
    
    //定时器清零
    TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
    Power = NextPower;
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
#define CMD_SENDDATA   13
#define CMD_FORCEON    27
#define CMD_FORCEOFF   37


ISR(PCINT0_vect){//先送高，后送低
  sei();
  if(drCLK)//上升沿读取
  {
    if(TempDataCnt==8)
    {
      if(drDAT)//起始位
      {
        TempData = 0;
        TempDataCnt = 0;
      }
    }
    else
    {
      TempData<<=1;
      if(drDAT)
      {
        TempData|=1;
      }
      
      {
      ///if(OutData&1)
      ///{
      ///  dwPORTON;
      ///}
      ///else
      ///{
      ///  dwPORTOFF;
      ///}
      ///OutData>>=1;
      }
      
      TempDataCnt++;
      if(TempDataCnt==8)
      {
        if(CMD==0)
        {
          switch(TempData)
          {
            case CMD_SENDDATA:
              DataReceiveCnt = 0;
              TargetRPMBuff = 0;
              rpmSend = rpm;
              CMD = TempData;
              return;
            case CMD_FORCEON:
              FStart=1;
              if(TIFR1 & _BV(TOV1))
              {
                //已超时，重启
                //.....
              }
              return;
            case CMD_FORCEOFF:
              FStart=0;
              return;
          }
        }
        else if(CMD == CMD_SENDDATA)
        {
          //High byte first
          TargetRPMBuff <<= 8;
          TargetRPMBuff |= TempData;
          
          OutData = (uint8_t)rpmSend;
          rpmSend >>= 8;
          
          DataReceiveCnt++;
          if(DataReceiveCnt==2)
          {
            TargetRPM = TargetRPMBuff;
            CMD = 0;
          }
        }
      }
    }
  }
}
