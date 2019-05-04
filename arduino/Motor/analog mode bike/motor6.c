#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

#define POWER_IN 6

#define PIN_breakBTN (!(PINA & _BV(7)))
#define PCINT_breakBTN _BV(PCINT7)

volatile uint8_t aread = 0;
volatile uint8_t pwrOn = 0;
volatile uint8_t pwrOff = 0;
volatile uint8_t timerStatus = 0;


int main(void) {
  //��ʼ��ʱ�ӣ�1MHz -> 8MHz
  CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
  CLKPR = 0;//1/1 //8MHz
  //��ʼ����ʱ�� 1/8
  TCCR1B = 2;//  1/8	1MHz 1us

  //��ʼ����ʱ��0
  TCCR0A = 0;
  TCCR0B = 2; // 1/1 8Mhz  31.250 Khz PWM
  TIMSK0 |= _BV(OCIE0A) | _BV(TOIE0);
  OCR0A = 128;

  ADMUX = POWER_IN;
  DIDR0 |= _BV(POWER_IN);
  ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2) | _BV(ADIE) | _BV(ADATE);
  ADCSRB = _BV(ADLAR);
  
  //������˿�
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //��ʼ������˿�
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();
  uint8_t Step = 0;
  uint16_t rpm;
  
  //��ѭ��
  for(;;) 
  {
    //������
    {
      uint8_t valbase = DigitReadBaseVal[Step];
      uint8_t drMask = DigitRead[Step];
      while(
        ((PIN3I&drMask)==valbase) && 
        (!(TIFR1 & _BV(TOV1)))
        );
    }
    
    //��⵽���� ���� �嶨ʱ��
    uint8_t Power;
    uint8_t Break;
    if(aread>=128){
      Break = 0;
      Power = (aread-128)<<1;
    }else if(aread>=121){
      Break = 0;
      Power = 0;
    }else{
      Break = 1;
      Power = (120-aread)<<1;
    }
    OCR0A = Power;
    
    {
      cli();
      if(PIN_breakBTN){
        if(Break){
          pwrOn = PWR_BREAK[Step];
          pwrOff = PWR_OFF[Step];
        } else {
          pwrOn = PWR_ON[Step];
          if(Power>=254){
            pwrOff = PWR_ON[Step];
          } else {
            pwrOff = PWR_OFF[Step];
          }
        }
      } else {
        pwrOn = PWR_OFF[Step];
        pwrOff = PWR_OFF[Step];
      }
      
      
      if(timerStatus){
        PORT6O = pwrOn;
      } else {
        PORT6O = pwrOff;
      }
      sei();
      
      //��¼��ǰת��
      rpm = ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1);
      //��ʱ������
      TCNT1 = 0;//TIFR1 |= _BV(TOV1);timer reset //overflow flg reset
      TIFR1 |= _BV(TOV1);
    }
    
    //�ȴ�30��
    while(TCNT1<(rpm>>1));
    
    //����
    {
      cli();
      Step = NextStep[Step];
      if(PIN_breakBTN){
        if(Break){
          pwrOn = PWR_BREAK[Step];
          pwrOff = PWR_OFF[Step];
        } else {
          pwrOn = PWR_ON[Step];
          if(Power>=254){
            pwrOff = PWR_ON[Step];
          } else {
            pwrOff = PWR_OFF[Step];
          }
        }
      } else {
        pwrOn = PWR_OFF[Step];
        pwrOff = PWR_OFF[Step];
      }
      
      if(timerStatus){
        PORT6O = pwrOn;
      } else {
        PORT6O = pwrOff;
      }
      sei();
    }
    
    uint16_t temp = rpm - (rpm>>3);//+(rpm>>2)+TCNT1;
    while(TCNT1<temp);//������ӳ�15(22.5)�Ⱥ��⡰���㡱
    
    //�ػ� ���߲��ػ�
    if(Power<254){
      cli();
      pwrOn = PWR_OFF[Step];
      pwrOff = PWR_OFF[Step];
      PORT6O = PWR_OFF[Step];
      sei();
    }
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

ISR(ADC_vect){
  aread = ADCH;
}
