#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "config.h"


//a6 start signal / restartup signal
//a7 throttle

#define POWER_IN 6

#define PIN_startBTN (!(PINA & _BV(7))) //低有效 接上拉电阻
#define PCINT_startBTN _BV(PCINT7)

#define FLIPSIGN   DDRB ^= _BV(3);
#define PWROn   DDRB |= _BV(3);/* */
#define PWROff  DDRB &= ~_BV(3);/**/

#define StartRpm 8192

uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};

uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

volatile uint8_t Step = 0;
volatile uint8_t FStart = 0;
volatile uint8_t aread;
volatile uint8_t Status = 0;//0 halt ,1 running, 2 starting
volatile uint8_t noskip = 1;

uint16_t rpm;
uint16_t Power = 0;
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
  PCMSK0 |= PCINT_startBTN;//start button

  ADMUX = POWER_IN;//A7 power
  DIDR0 |= _BV(POWER_IN);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = _BV(ADLAR);
  
  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  PORTB &= ~_BV(3);
  
  sei();
  //主循环
  for(;;) 
  {
    //定时器清零
    TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
    Power = NextPower;
    uint8_t tempStep = Step;
    if(Power)
    {
      PORT6O = PWR_ON[tempStep];//PWROn;
    }
    else
    {
      PORT6O = PWR_OFF[tempStep];//PWROff;
    }
    OCR1A = Power;
    //??? if(TCNT1>OCR1A)PORT6O = PWR_OFF[tempStep];
    //转速调整
    adj();
    FLIPSIGN;
    //等待过零
    {
      uint8_t valbase = DigitReadBaseVal[tempStep];
      uint8_t drMask = DigitRead[tempStep];
      
      uint16_t temp = (rpm>>1);
      while(TCNT1<temp);
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
    }
    if(!PIN_startBTN)
    {
      uint16_t tmp = (rpm>>3)+(rpm>>2)+TCNT1;
      while(TCNT1<tmp);
    }
    PORT6O = PWR_OFF[Step];
    Step = NextStep[Step];//CmdNextStep;
    //记录当前转速
    rpm = TCNT1;
  }
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];
}

uint16_t _MaxPower(uint16_t val){
  uint32_t temp = val;
  uint32_t temp2 = 512+128+aread;//256+(0-1024)   256+512  512+128
  if(temp>temp2){
    return temp-temp2;
  }else{
    return 0;
  }
}

void adj(){
  NextPower = 0;
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      Status = 0;
    }
    else
    {
      NextPower = _MaxPower(rpm);
    }
  }
  else
  {
    if(PIN_startBTN)
    {
      NextPower = _MaxPower(rpm);
    }
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
      Status = 1;
    }
  }
}

ISR(ADC_vect){
  aread = ADCH;
}

