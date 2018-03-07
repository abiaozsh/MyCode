#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//a6 start signal / restartup signal
//a7 throttle

#define POWER_IN 6

#define PIN_startBTN (!(PINA & _BV(7)))
#define PCINT_startBTN _BV(PCINT7)

#define CPUFree ;/*DDRB |= _BV(3) */ //free亮灯
#define CPUBusy ;/*DDRB &= ~_BV(3)*/ //busy暗灯
#define STAOn   DDRB |= _BV(3) ;/**/
#define STAOff  DDRB &= ~_BV(3);/**/
#define PWROn   ;/*DDRB |= _BV(3) */
#define PWROff  ;/*DDRB &= ~_BV(3)*/
#define RPMFlip ;/*DDRB ^= _BV(3)*/

#define DBGOn   ;/*DDRB |= _BV(3) */
#define DBGOff  ;/*DDRB &= ~_BV(3)*/

#define StartRpm 8192

volatile uint8_t Step = 0;
volatile uint8_t Status = 0;
volatile uint8_t noskip = 1;
volatile uint8_t Power = 0;

uint16_t rpm = 10000;

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  //TIMSK1 = _BV(OCIE1A);

  TCCR0B = 3;//488.28125 Hz PWM
  TIMSK0 = _BV(OCIE0A) + _BV(TOIE0);
  
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
  //主循环
  for(;;) 
  {
    //定时器清零
    TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
    //Power = NextPower;
    uint8_t tempStep = Step;
    if(Power)
    {
      PORT6O = PWR_ON[tempStep];PWROn;//CmdPWROn;
    }
    else
    {
      PORT6O = PWR_OFF[tempStep];PWROff;//CmdPWROff;
    }
    //OCR1A = Power;
    //adj();
    if(Status)
    {
      if(rpm>StartRpm)//too slow, halt
      {
        Status = 0;STAOff;
      }
    }
    RPMFlip;
    //等待“过零”
    uint8_t valbase = DigitReadBaseVal[tempStep];
    uint8_t drMask = DigitRead[tempStep];
    if(PIN_startBTN)
    {
      uint16_t temp = (rpm>>1);
      CPUFree;
      while(TCNT1<temp);//换向后延迟30度后检测“过零”
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
      CPUBusy;
	  //检测到过零后，立即换向
    }
    else
    //等待“过零”
    {
      uint16_t temp = (rpm>>2);//(rpm>>3)+
      CPUFree;
      while(TCNT1<temp);//换向后延迟15(22.5)度后检测“过零”
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
      CPUBusy;
    
      //检测到过零后，再等待30度
    
      uint16_t tmp = (rpm>>1)+TCNT1;
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

ISR(TIM0_COMPA_vect){
  PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
  Power = 0;
}

ISR(TIM0_OVF_vect){
  if((PIN_startBTN || Status) && OCR0A>10){
    PORT6O = PWR_ON[Step];PWROn;
    Power = 1;
  }
  else
  {
    Power = 0;
  }
}

ISR(PCINT0_vect){
  if(noskip)
  {
    if(PIN_startBTN)
    {
      noskip = 0;
    }
    else
    {
      Status = 1;STAOn;
    }
  }
}

ISR(ADC_vect){
  OCR0A = ADCH;
}

