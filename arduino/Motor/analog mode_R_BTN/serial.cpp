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
#define BP2B _BV(1)

#define BP3U _BV(6)
#define BP3D _BV(7)
#define BP3A _BV(2)
#define BP3B _BV(2)


//下臂常开，反驱
uint8_t PWR_ON6O[] = {
  BP1U + BP2D, // 1-2 a
  BP1U + BP3D, // 1-3 b
  BP2U + BP3D, // 2-3 c
  BP2U + BP1D, // 2-1 d
  BP3U + BP1D, // 3-1 e
  BP3U + BP2D  // 3-2 f
};

uint8_t RECYCLE6O[] = {
  0 + BP2D, // 1-2 a
  0 + BP3D, // 1-3 b
  0 + BP3D, // 2-3 c
  0 + BP1D, // 2-1 d
  0 + BP1D, // 3-1 e
  0 + BP2D  // 3-2 f
};
uint8_t RECYCLE3O[] = {
  BP1B, // 1-2 a
  BP1B, // 1-3 b
  BP2B, // 2-3 c
  BP2B, // 2-1 d
  BP3B, // 3-1 e
  BP3B  // 3-2 f
};


uint8_t PWR_OFF6O[] = {
  0    + BP2D, // 1-2
  0    + BP3D, // 1-3
  0    + BP3D, // 2-3
  0    + BP1D, // 2-1
  0    + BP1D, // 3-1
  0    + BP2D  // 3-2
};

#define STAOn   PORTB |= _BV(5) ;/**/
#define STAOff  PORTB &= ~_BV(5);/**/

//????
#define StartRpm (8192*3)

uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};

uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

volatile uint8_t Step = 0;
volatile uint8_t aval;
volatile uint8_t PowerType;
volatile uint8_t Status = 0;//0 halt ,1 running, 2 starting
volatile uint8_t noskip = 1;
#define StartBtn (!(PINB & _BV(3)))
//volatile uint8_t StartBtn = 0;

uint16_t rpm;
uint16_t Power = 0;
uint16_t NextPower = 0;
uint8_t NextPowerType;

uint8_t CMD = 0;
uint8_t TempData=0;
uint8_t TempDataCnt=8;

void adj();

int main(void) {
  //CLKPR = 0;//1/1 //16MHz
  //初始化定时器 1/8
  TCCR1B = 2;//  1/8	2MHz 0.5us
  TIMSK1 |= _BV(OCIE1A);
  //初始化输入
  PCICR |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT3);

  ADMUX = _BV(REFS0) | 3 | _BV(ADLAR);//A3 power
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = 0;

  //打开输出端口
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  DDR3O = BP1B | BP2B | BP3B;
  DDRB |= _BV(5);
  DDRC |= _BV(5);
  DDRC |= _BV(4);
  
  sei();
  //主循环
  for(;;) 
  {
    //定时器清零
    TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
    uint8_t tempStep = Step;
    if(NextPowerType == 1)//Power on
    {
      PORT6O = PWR_ON6O[tempStep];
    }
    else if(NextPowerType == 2)//Recycle
    {
      PORT6O = RECYCLE6O[tempStep];
      PORT3O |= RECYCLE3O[tempStep];
    }
    else//Power off
    {
      PORT6O = PWR_OFF6O[tempStep];
    }
    OCR1A = NextPower;
    //转速调整
    adj();
    //等待过零
    {
      uint8_t valbase = DigitReadBaseVal[tempStep];
      uint8_t drMask = DigitRead[tempStep];
      
      uint16_t temp = (rpm>>1);
      while(TCNT1<temp);
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
    }
    if(!StartBtn)
    {
      uint16_t tmp = (rpm>>3)+(rpm>>2)+TCNT1;
      while(TCNT1<tmp);
    }
    PORT3O &= ~(BP1B | BP2B | BP3B);
    PORT6O = PWR_OFF6O[Step];
    Step = NextStep[Step];//CmdNextStep;
    //记录当前转速
    rpm = TCNT1;
  }
}

ISR(TIMER1_COMPA_vect){
  PORT3O &= ~(BP1B | BP2B | BP3B);
  PORT6O = PWR_OFF6O[Step];
}

void _CalcPower(){
  uint32_t temp = rpm;
  //28 耗时
  //224clock
  temp*=aval;
  temp>>=8;
  NextPower = (uint16_t)temp;
  NextPowerType = PowerType;
}

void adj(){
  NextPower = 0;
  NextPowerType = 0;
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      Status = 0;STAOff;
    }
    else
    {
      _CalcPower();
    }
  }
  else
  {
    if(StartBtn)
    {
      _CalcPower();
    }
	}
}

ISR(PCINT0_vect){
  if(noskip)
  {
    if(StartBtn)
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
  uint8_t aread = ADCH;
  if (aread > 128)
  {
    aval = (aread - 128)<<1;//0~256
    PowerType = 1;
    PORTC &= ~_BV(4);
    PORTC |=  _BV(5);
  }
  else if (aread < 64)
  {
    aval = (64 - aread)<<1;//0~128
    PowerType = 2;
    PORTC |=  _BV(4);
    PORTC &= ~_BV(5);
  }
  else
  {
    aval = 0;
    PowerType = 0;
    PORTC &= ~_BV(4);
    PORTC &= ~_BV(5);
  }
}
