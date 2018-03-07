#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "../config.h"

//��������

//drA3
//gnd
//vcc
#define drDAT (PINA & _BV(7)) /*DAT*/
#define drCLK (PINA & _BV(6)) /*DAT*/

#define CPUFree ;/*DDRB |= _BV(3) */ //free����
#define CPUBusy ;/*DDRB &= ~_BV(3)*/ //busy����
#define STAOn   ;/*DDRB |= _BV(3) */
#define STAOff  ;/*DDRB &= ~_BV(3)*/
#define FSTOn   ;/*DDRB |= _BV(3) */
#define FSTOff  ;/*DDRB &= ~_BV(3)*/
#define PWROn   DDRB |= _BV(3) ;/**/
#define PWROff  DDRB &= ~_BV(3);/**/
#define RPMFlip ;/*DDRB ^= _BV(3)*/

#define StartRpm 8192

volatile uint8_t Step = 0;
volatile uint8_t FStart = 0;
volatile uint8_t MaxPower = 0;
volatile uint8_t Pitch = 1;
volatile uint8_t noskip = 1;
volatile uint8_t Status = 0;//0 halt ,1 running, 2 starting

uint16_t rpm;
uint16_t NextPower = 0;

uint8_t CMD = 0;
uint8_t TempData=0;
uint8_t TempDataCnt=8;


inline void _MaxPower(){
  if(MaxPower==255){NextPower = 4096;return;}//StartRpm ��һ��
  if(MaxPower==0){NextPower = 0;return;}//StartRpm ��һ��
  uint32_t temp = rpm;
  //28 ��ʱ
  //224clock
  temp*=MaxPower;
  temp>>=8;
  uint16_t temp2 = temp;
  if(temp2<50){NextPower = 0;return;}
  NextPower = temp2;
  if(NextPower>4096)NextPower = 4096;
}

inline void adj(){
  NextPower = 0;
  if(Status)
  {
    if(rpm>StartRpm)//too slow, halt
    {
      Status = 0;STAOff;
    }
    else
    {
      _MaxPower(rpm);
    }
  }
  else
  {
    if(FStart)
    {
      _MaxPower(rpm);
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
  //��ʼ������
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT6);//CLK

  //������˿�
  DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
  
  //��ʼ������˿�
  PORT6O = BP1D | BP2D | BP3D;
  
  sei();
  //��ѭ��
  for(;;) 
  {
    //��ʱ������
    TCNT1 = 0;
    uint16_t Power = NextPower;
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
    //ת�ٵ���
    adj();
    RPMFlip;
    //�ȴ�����
    uint8_t valbase = DigitReadBaseVal[tempStep];
    uint8_t drMask = DigitRead[tempStep];
    if(FStart)
    {
      uint16_t temp = (rpm>>1);
      CPUFree;
      while(TCNT1<temp);//������ӳ�30�Ⱥ��⡰���㡱
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
      CPUBusy;
	  //��⵽�������������
    }
    else
    //�ȴ������㡱
    {
      uint16_t temp = (rpm>>2);//(rpm>>3)+
      CPUFree;
      while(TCNT1<temp);//������ӳ�15(22.5)�Ⱥ��⡰���㡱
      noskip = 1;
      while(((PIN3I&drMask)==valbase) && noskip);
      CPUBusy;
    
      //��⵽������ٵȴ�30��
      uint16_t tmp = (rpm>>1)+TCNT1;
      CPUFree;
      while(TCNT1<tmp);
      CPUBusy;
    }



    PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
    Step = NextStep[Step];//CmdNextStep;
    //��¼��ǰת��
    rpm = TCNT1;
  }
}

ISR(TIM1_COMPA_vect){
  PORT6O = PWR_OFF[Step];PWROff;//CmdPWROff;
}

#define CMD_START         1  /*START          */
#define CMD_NOSTART       2  /*START off     */
#define CMD_SETPWRSIMP    3  /*set power simple */
#define CMD_PITCH         4  /*PITCH         */

ISR(PCINT0_vect){//���͸ߣ����͵�
  if(TempDataCnt == 8)
  {
    if(drDAT)//��ʼλ
    {
      TempData = 0;
      TempDataCnt = 0;
    }
  }
  else
  {
    TempData <<= 1;
    if(drDAT)
    {
      TempData |= 1;
    }
    TempDataCnt++;
    if(TempDataCnt == 8)
    {
      if(CMD == 0)
      {
        CMD = TempData;
      }
      else
      {
        switch(CMD)
        {
          case CMD_START:
          {
            FStart = 1;FSTOn;
            noskip = 0;
            break;
          }
          case CMD_NOSTART:
          {
            FStart = 0;FSTOff;
            Status = 1;STAOn;
            break;
          }
          case CMD_SETPWRSIMP:
            MaxPower = TempData;
            break;
          case CMD_PITCH:
            Pitch = TempData;
            break;
        }
        CMD = 0;
      }
    }
  }
}
