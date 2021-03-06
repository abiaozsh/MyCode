#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//#define SOFT_START_BREAK

#define POWER_IN 6

#ifdef SOFT_START_BREAK
#else
#define PIN_startBTN (!(PINA & _BV(7)))
#define PCINT_startBTN _BV(PCINT7)
#endif

#define DBGOn   ;/*DDRB |= _BV(3) */
#define DBGOff  ;/*DDRB &= ~_BV(3)*/

#define StartRpm 8192


volatile uint8_t Step = 0;
volatile uint8_t PowerState = 0;
volatile uint8_t aread = 255;
volatile uint8_t Status = 0;
volatile uint8_t noskip = 1;
volatile uint8_t Startup = 0;
volatile uint8_t Break = 0;

volatile uint8_t pwrOn = 0;
volatile uint8_t pwrOff = 0;
volatile uint8_t timerStatus = 0;

uint16_t rpm;
uint16_t Power = 0;
uint16_t NextPower = 0;


inline void _MaxPower(){
  uint8_t temparead = aread;
  #ifdef SOFT_START_BREAK
  if(temparead>=128){
    Break = 0;
    temparead = temparead - 128;
    temparead = temparead << 1;
    temparead = temparead + 1;
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
  else
  {
    if(temparead<64)
    {
      Break = 1;
      temparead = 64 - temparead;
      temparead = temparead << 1;
      if(temparead==0){
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
  }
  #else
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
  #endif
}

inline void adj(){
  NextPower = 0;
  if(Status)
  {
    if(Status>1){
      Status--;
      NextPower = 0;
    }
    else
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
}

void startup(){
  Status = 6;
  pwrOn = PWR_ON[Step];
  pwrOff = PWR_OFF[Step];
 
  //初始化定时器0
  TCCR0A = 0;
  TCCR0B = 1; // 1/1 8Mhz  31.250 Khz PWM
  TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0);
  OCR0A = 0;

  uint8_t cnt = 0;
  uint8_t total;
  
  if((PINB & _BV(3)))
  {
    rpm = 10000;
    total = 70;
  }
  else
  {
    rpm = 65000;
    total = 200;
  }
  
  uint8_t tempStep = Step;
  for(cnt=0;cnt<total;cnt++)
  {
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
      cli();
      pwrOn = PWR_ON[tempStep];
      pwrOff = PWR_OFF[tempStep];
      if(timerStatus){
        PORT6O = pwrOn;
      }else
      {
        PORT6O = pwrOff;
      }
      sei();
      //
      rpm = rpm - (rpm>>6) - 1;//TCNT1;
      //定时器清零
      TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
      //OCR1A = Power;
    }
    //等待30度
    while(TCNT1<(rpm>>1));
    
    if(cnt==total-1)
    {
      OCR0A = 0;
      TIMSK0 = 0;
      timerStatus = 0;
    }
    else
    {
      uint8_t temparead = aread;
      #ifdef SOFT_START_BREAK
      if(temparead>=128){
        temparead = temparead - 128;
        temparead = temparead << 1;
        temparead++;
        OCR0A = temparead;
      }
      else
      {
        OCR0A = 1;
      }
      #else
        if(temparead<32){
			temparead = 32;
		}
        OCR0A = temparead;
      #endif
    }
    //换向
    {
      cli();
      Step = NextStep[Step];
      tempStep = Step;
      pwrOn = PWR_ON[tempStep];
      pwrOff = PWR_OFF[tempStep];
      if(timerStatus){
        PORT6O = pwrOn;
      }else
      {
        PORT6O = pwrOff;
      }
      sei();
    }
    
    uint16_t temp = (rpm>>2)+TCNT1;
    while(TCNT1<temp);//换向后延迟15(22.5)度后检测“过零”
  }

  OCR0A = 0;
  TIMSK0 = 0;
  NextPower = 0;
}

ISR(TIM0_OVF_vect){
  PORT6O = pwrOn;
  timerStatus = 1;
}

ISR(TIM0_COMPA_vect){
  PORT6O = pwrOff;
  timerStatus = 0;
}

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  TIMSK1 |= _BV(OCIE1A);

  #ifdef SOFT_START_BREAK
  #else
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= PCINT_startBTN;//start button
  #endif

  ADMUX = POWER_IN;
  DIDR0 |= _BV(POWER_IN);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = _BV(ADLAR);
  
  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();
  
  #ifdef SOFT_START_BREAK
  while(aread>(128+8));
  #else
  while(aread>(16));
  #endif
  
  //主循环
  uint8_t tempStep = Step;
  for(;;) 
  {
    //启动序列
    if(Startup)
    {
      startup();
      Startup=0;
      tempStep = Step;
    }

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
        if(Break){
          PORT6O = PWR_BREAK[tempStep];
        }else
        {
          PORT6O = PWR_ON[tempStep];
        }
        PowerState = 1;
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];
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
      Step = NextStep[Step];
      tempStep = Step;
      if(PowerState)
      {
        if(Break){
          PORT6O = PWR_BREAK[tempStep];
        }else
        {
          PORT6O = PWR_ON[tempStep];
        }
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];
      }
      sei();
    }
    
    uint16_t temp = rpm - (rpm>>3);//+(rpm>>2)+TCNT1;
    while(TCNT1<temp);//换向后延迟15(22.5)度后检测“过零”
  }
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];
  PowerState = 0;
}

#ifdef SOFT_START_BREAK
#else
ISR(PCINT0_vect){
  if(noskip)
  {
    if(PIN_startBTN)
    {
      Startup = 1;
      noskip = 0;
    }
  }
}
#endif

ISR(ADC_vect){
  aread = ADCH;
  #ifdef SOFT_START_BREAK
  if((!Status) && aread>(128+8)){
    Startup = 1;
    noskip = 0;
  }
  #endif
}
