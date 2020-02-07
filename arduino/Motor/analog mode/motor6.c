#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//a6 start signal / restartup signal
//a7 throttle
#define CPUFree ;/*DDRB |=  _BV(3)*/ //free亮灯
#define CPUBusy ;/*DDRB &= ~_BV(3)*/ //busy暗灯
#define STAOn   ;/*DDRB |=  _BV(3)*/
#define STAOff  ;/*DDRB &= ~_BV(3)*/
#define PWROn   ;/*DDRB |=  _BV(3)*/
#define PWROff  ;/*DDRB &= ~_BV(3)*/
#define RPMFlip ;/*DDRB ^=  _BV(3)*/

#define POWER_IN 6

#define PIN_startBTN (!(PINA & _BV(7)))
#define PCINT_startBTN _BV(PCINT7)

#define DBGOn   ;/*DDRB |= _BV(3) */
#define DBGOff  ;/*DDRB &= ~_BV(3)*/

#define StartRpm 8192


volatile uint8_t Step = 0;
volatile uint8_t PowerState = 0;
volatile uint8_t aread;
volatile uint8_t Status = 0;
volatile uint8_t noskip = 1;


uint16_t rpm;
uint16_t Power = 0;
uint16_t NextPower = 0;


inline void _MaxPower(){
  uint8_t temparead = aread;
  if(temparead==255){
	  NextPower = 4096;
  }else if(temparead==0){
	  NextPower = 0;
  }else{
	  uint32_t temp = rpm;
	  //28 耗时
	  //224clock
	  temp*=temparead;
	  temp>>=8;
	  uint16_t temp2 = temp;
	  NextPower = temp2;
	  if(NextPower>4096)NextPower = 4096;
  }
}

inline void adj(){
  NextPower = 0;
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      Status = 0;
    }
    else
    {
      _MaxPower();
    }
  }
}

void startup(){


  uint8_t cnt = 0;
  
  rpm = 12000;
  
  for(cnt=0;cnt<100;cnt++)
  {
    uint16_t temp = (rpm>>2)+TCNT1;
    while(TCNT1<temp);//换向后延迟15(22.5)度后检测“过零”

    uint8_t tempStep = Step;
    //检测过零
    //{
    //  uint8_t valbase = DigitReadBaseVal[tempStep];
    //  uint8_t drMask = DigitRead[tempStep];
    //  noskip = 1;
    //  while(((PIN3I&drMask)==valbase) && noskip);
    //}
    uint16_t temp2 = (rpm>>1)+TCNT1;
    while(TCNT1<temp2);//换向后延迟15(22.5)度后检测“过零”
    
    //检测到过零 开机 清定时器
    {
      Power = NextPower;
      if(Power)
      {
        PORT6O = PWR_ON[tempStep];
        PowerState = 1;
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];
        PowerState = 0;
      }
      //记录当前转速
      rpm = rpm - (rpm>>6) - 1;//TCNT1;
      //定时器清零
      TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
      OCR1A = Power;
    }
    
    //功率计算
    NextPower = 0;
    _MaxPower();
    
    //等待30度
    while(TCNT1<(rpm>>1));
    
    //换向
    {
      cli();
      Step = NextStep[Step];//CmdNextStep;
      tempStep = Step;
      if(PowerState)
      {
        PORT6O = PWR_ON[tempStep];PWROn;//CmdPWROn;
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];PWROff;//CmdPWROff;
      }
      sei();
    }
  }
  Status = 1;STAOn;

}

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  TIMSK1 |= _BV(OCIE1A);
  //初始化输入
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= PCINT_startBTN;//start button

  ADMUX = POWER_IN;//A7 power
  DIDR0 |= _BV(POWER_IN);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = _BV(ADLAR);
  
  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();

  while(aread>(32));

  //主循环
  for(;;) 
  {
    //启动序列
    if(PIN_startBTN)
    {
      startup();
    }

    uint8_t tempStep = Step;
    //检测过零
    {
      uint8_t valbase = DigitReadBaseVal[tempStep];
      uint8_t drMask = DigitRead[tempStep];
      noskip = 1;
      while(
        ((PIN3I&drMask)==valbase) && 
        noskip && 
        (!(TIFR1 & _BV(TOV1)))
        );
    }
    
    //检测到过零 开机 清定时器
    {
      Power = NextPower;
      if(Power)
      {
        PORT6O = PWR_ON[tempStep];PWROn;//CmdPWROn;
        PowerState = 1;
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];PWROff;//CmdPWROff;
        PowerState = 0;
      }
      //记录当前转速
      rpm = ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1);
      //定时器清零
      TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
      TIFR1 |= _BV(TOV1);
      OCR1A = Power;
    }
    
    //功率计算
    adj();
    
    //等待30度
    while(TCNT1<(rpm>>1));
    
    //换向
    {
      cli();
      Step = NextStep[Step];//CmdNextStep;
      tempStep = Step;
      if(PowerState)
      {
        PORT6O = PWR_ON[tempStep];PWROn;//CmdPWROn;
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];PWROff;//CmdPWROff;
      }
      sei();
    }
    
    uint16_t temp = (rpm>>3)+(rpm>>2)+TCNT1;
    while(TCNT1<temp);//换向后延迟15(22.5)度后检测“过零”
  }
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];
  PowerState = 0;
}

ISR(PCINT0_vect){
  if(noskip)
  {
    if(PIN_startBTN)
    {
      noskip = 0;
    }
  }
}

ISR(ADC_vect){
  aread = ADCH;
}
