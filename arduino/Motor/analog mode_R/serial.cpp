#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define PORT6O PORTD
#define DDR6O DDRD

#define PORT3O PORTB
#define DDR3O DDRB

#define PIN3I PINC
#define DDR3I DDRC

#define BP1U _BV(2)
#define BP1D _BV(3)
#define BP1A _BV(0)
#define BP1B _BV(0)

#define BP2U _BV(4)
#define BP2D _BV(5)
#define BP2A _BV(1)
#define BP2B _BV(0)

#define BP3U _BV(6)
#define BP3D _BV(7)
#define BP3A _BV(2)
#define BP3B _BV(0)


//下臂常开，反驱
uint8_t PWR_ON[] = {
  BP1U + BP2D, // 1-2 a
  BP1U + BP3D, // 1-3 b
  BP2U + BP3D, // 2-3 c
  BP2U + BP1D, // 2-1 d
  BP3U + BP1D, // 3-1 e
  BP3U + BP2D  // 3-2 f
};
uint8_t PWR_OFF[] = {
  0    + BP2D, // 1-2
  0    + BP3D, // 1-3
  0    + BP3D, // 2-3
  0    + BP1D, // 2-1
  0    + BP1D, // 3-1
  0    + BP2D  // 3-2
};



#define drP6 (!(PINB & _BV(3)))

#define CPUFree ;/*DDRB |= _BV(3) */ //free亮灯
#define CPUBusy ;/*DDRB &= ~_BV(3)*/ //busy暗灯
#define STAOn   DDRB |= _BV(5) ;/**/
#define STAOff  DDRB &= ~_BV(5);/**/
#define PWROn   ;/*DDRB |= _BV(3) */
#define PWROff  ;/*DDRB &= ~_BV(3)*/
#define RPMFlip ;/*DDRB ^= _BV(3)*/

#define DBGOn   ;/*DDRB |= _BV(3) */
#define DBGOff  ;/*DDRB &= ~_BV(3)*/

//a6 start signal / restartup signal
//a7 throttle

#define StartRpm (8192*2)

uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};

uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

volatile uint8_t Step = 0;
volatile uint8_t FStart = 0;
volatile uint8_t aread;
volatile uint8_t areadB;
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
  //CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  //CLKPR = 0;//1/1 //8MHz
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  TIMSK1 |= _BV(OCIE1A);
  //初始化输入
  PCICR |= _BV(PCIE0);//GIMSK |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT3);//start button

  //ADMUX = 3;//A3 power
  //ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  //ADCSRB = _BV(ADLAR);
  
  ADMUX = _BV(REFS0) | 3 | _BV(ADLAR);//A3 power
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = 0;

  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //初始化输出端口
  PORT6O = BP1D | BP2D | BP3D;
  
  DDRB |= _BV(5) ;
  DDRB |= _BV(4) ;
  DDRC |= _BV(4) ;
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
    if(!drP6)
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

ISR(TIMER1_COMPA_vect){
  PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
}

uint16_t _MaxPower(uint16_t val){
  uint32_t temp = val;
  //28 耗时
  //224clock
  temp*=aread;
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
    if(drP6)
    {
      NextPower = _MaxPower(rpm);
    }
	}
}

ISR(PCINT0_vect){
  if(noskip)
  {
    if(drP6)
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
uint8_t val;
  val = ADCH;
  if(val>=128){
    aread = (val - 128)<<1;
    areadB = 0;
    PORTB|=_BV(4);
    PORTC&=~_BV(4);
  }
  else if(val<=64){
    areadB = (64 - val);
    aread = 0;
    PORTC|=_BV(4);
    PORTB&=~_BV(4);
  }
  else
  {
    areadB =0;
    aread = 0;
    PORTC&=~_BV(4);
    PORTB&=~_BV(4);
  }
}
