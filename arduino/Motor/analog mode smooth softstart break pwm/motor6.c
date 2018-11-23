#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//a6 start signal / restartup signal
//a7 throttle

#define POWER_IN 6


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
uint8_t fullpwm = 0;


inline void _MaxPower(){
  uint8_t temparead = aread;
  fullpwm = 1;
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
      if(temparead<128){
        fullpwm = 0;
        OCR0A = temparead << 1;// * 2
        NextPower = rpm >> 1;
      }
      else
      {
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
  else
  {
    Break = 1;
    temparead = 128 - temparead;
    fullpwm = 0;
    OCR0A = temparead + (temparead>>1);// * 2
    NextPower = rpm - (rpm>>2);
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
  Status = 1;

  uint8_t cnt = 0;
  
  rpm = 12000;
  
  for(cnt=0;cnt<100;cnt++)
  {
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
        if(Break){
          pwrOn = PWR_BREAK[tempStep];
        }else
        {
          pwrOn = PWR_ON[tempStep];
        }
        PowerState = 1;
      }
      else
      {
        pwrOn = PWR_OFF[tempStep];
        PowerState = 0;
      }
      if(fullpwm)
      {
        pwrOff = pwrOn;
      }
      else
      {
        pwrOff = PWR_OFF[tempStep];
      }
      if(timerStatus){
        PORT6O = pwrOn;
      }else
      {
        PORT6O = pwrOff;
      }
      //
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
      Step = NextStep[Step];
      tempStep = Step;
      if(PowerState)
      {
        if(Break){
          pwrOn = PWR_BREAK[tempStep];
        }else
        {
          pwrOn = PWR_ON[tempStep];
        }
      }
      else
      {
        pwrOn = PWR_OFF[tempStep];
      }
      if(fullpwm)
      {
        pwrOff = pwrOn;
      }
      else
      {
        pwrOff = PWR_OFF[tempStep];
      }
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
  
  
}

int main(void) {
  //初始化时钟：1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  
  //初始化定时器1 1/8
  TCCR1A = 0;
  TCCR1B = 2;//  1/8	1MHz 1us
  TIMSK1 |= _BV(OCIE1A);

  //初始化定时器0
  TCCR0A = 0;
  TCCR0B = 1; // 1/1 8Mhz  31250 Khz PWM
  TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0);
  OCR0A = 0;
  
  ADMUX = POWER_IN;//A7 power
  DIDR0 |= _BV(POWER_IN);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = _BV(ADLAR);
  
  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();
  
  while(aread>(128+32));
  //主循环
  for(;;) 
  {
    //启动序列
    if(Startup)
    {
      startup();
      Startup=0;
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
        if(Break){
          pwrOn = PWR_BREAK[tempStep];
        }else
        {
          pwrOn = PWR_ON[tempStep];
        }
        PowerState = 1;
      }
      else
      {
        pwrOn = PWR_OFF[tempStep];
        PowerState = 0;
      }
      if(fullpwm)
      {
        pwrOff = pwrOn;
      }
      else
      {
        pwrOff = PWR_OFF[tempStep];
      }
      if(timerStatus){
        PORT6O = pwrOn;
      }else
      {
        PORT6O = pwrOff;
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
          pwrOn = PWR_BREAK[tempStep];
        }else
        {
          pwrOn = PWR_ON[tempStep];
        }
      }
      else
      {
        pwrOn = PWR_OFF[tempStep];
      }
      if(fullpwm)
      {
        pwrOff = pwrOn;
      }
      else
      {
        pwrOff = PWR_OFF[tempStep];
      }
      if(timerStatus){
        PORT6O = pwrOn;
      }else
      {
        PORT6O = pwrOff;
      }
      sei();
    }
    
    uint16_t temp = rpm - (rpm>>3);//+(rpm>>2)+TCNT1;
    while(TCNT1<temp);//换向后延迟15(22.5)度后检测“过零”
  }
}

ISR(TIM0_OVF_vect){
  PORT6O = pwrOn;
  timerStatus = 1;
}

ISR(TIM0_COMPA_vect){
  PORT6O = pwrOff;
  timerStatus = 0;
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];
  pwrOn = PWR_OFF[Step];
  pwrOff = PWR_OFF[Step];
  PowerState = 0;
}

ISR(ADC_vect){
  aread = ADCH;
  if((!Status) && aread>(128+32)){
    Startup = 1;
    noskip = 0;
  }
}
