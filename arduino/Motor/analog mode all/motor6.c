#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//#define SOFT_START_BREAK
//#define CURRENT_LIMIT

#define POWER_IN 6


#ifdef CURRENT_LIMIT
#define PIN_CURLMT ((PINB & _BV(3)))
#define PCINT_CURLMT _BV(PCINT11)
#endif

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
      //28 ��ʱ
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
        //28 ��ʱ
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
	  //28 ��ʱ
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
  #ifdef CURRENT_LIMIT
  if(!PIN_CURLMT)
  #endif
  {
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
}

void startup(){
  Status = 1;

  uint8_t cnt = 0;
  
  rpm = 12000;
  
  for(cnt=0;cnt<100;cnt++)
  {
    uint16_t temp = (rpm>>2)+TCNT1;
    while(TCNT1<temp);//������ӳ�15(22.5)�Ⱥ��⡰���㡱

    uint8_t tempStep = Step;
    //������
    //{
    //  uint8_t valbase = DigitReadBaseVal[tempStep];
    //  uint8_t drMask = DigitRead[tempStep];
    //  noskip = 1;
    //  while(((PIN3I&drMask)==valbase) && noskip);
    //}
    uint16_t temp2 = (rpm>>1)+TCNT1;
    while(TCNT1<temp2);//������ӳ�15(22.5)�Ⱥ��⡰���㡱
    
    //��⵽���� ���� �嶨ʱ��
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
      //��¼��ǰת��
      rpm = rpm - (rpm>>6) - 1;//TCNT1;
      //��ʱ������
      TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
      OCR1A = Power;
    }
    
    //���ʼ���
    NextPower = 0;
    #ifdef CURRENT_LIMIT
    if(!PIN_CURLMT)
    #endif
    {
      _MaxPower();
    }
    
    //�ȴ�30��
    while(TCNT1<(rpm>>1));
    
    //����
    {
      cli();
      Step = NextStep[Step];
      tempStep = Step;
      if(PowerState)
      {
        PORT6O = PWR_ON[tempStep];
      }
      else
      {
        PORT6O = PWR_OFF[tempStep];
      }
      sei();
    }
  }
  
  
}

int main(void) {
  //��ʼ��ʱ�ӣ�1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //��ʼ����ʱ�� 1/8
  TCCR1B = 2;//  1/8	1MHz 1us
  TIMSK1 |= _BV(OCIE1A);

  #ifdef SOFT_START_BREAK
  #else
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= PCINT_startBTN;//start button
  #endif

  #ifdef CURRENT_LIMIT
  //��ʼ������
  GIMSK |= _BV(PCIE1);
  PCMSK1 |= PCINT_CURLMT;
  #endif

  ADMUX = POWER_IN;
  DIDR0 |= _BV(POWER_IN);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = _BV(ADLAR);
  
  //������˿�
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //��ʼ������˿�
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();
  
  #ifdef SOFT_START_BREAK
  while(aread>(128+32));
  #else
  while(aread>(32));
  #endif
  
  //��ѭ��
  for(;;) 
  {
    //��������
    if(Startup)
    {
      startup();
      Startup=0;
    }

    uint8_t tempStep = Step;
    //������
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
    
    //��⵽���� ���� �嶨ʱ��
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
      //��¼��ǰת��
      rpm = ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1);
      //��ʱ������
      TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
      TIFR1 |= _BV(TOV1);
      OCR1A = Power;
    }
    
    //���ʼ���
    adj();
    
    //�ȴ�30��
    while(TCNT1<(rpm>>1));
    
    //����
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
    while(TCNT1<temp);//������ӳ�15(22.5)�Ⱥ��⡰���㡱
  }
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];
  PowerState = 0;
}

#ifdef CURRENT_LIMIT
ISR(PCINT1_vect){
  if(PIN_CURLMT)
  {
    PORT6O = PWR_OFF[Step];
    PowerState = 0;
  }
}
#endif

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
  if((!Status) && aread>(128+32)){
    Startup = 1;
    noskip = 0;
  }
  #endif
}
