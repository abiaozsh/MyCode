#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define MaxPonTime 2000

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

//drA3
#define drDAT (PINA & _BV(7)) /*DAT*/
#define drCLK (PINA & _BV(6)) /*DAT*/

//2 1 0
//5 4 3 2 1 0
#define PORT6O PORTA
#define DDR6O DDRA

#define PORT3I PORTB
#define DDR3I DDRB
#define PIN3I PINB

#define BP1D _BV(5)
#define BP1U _BV(4)
#define BP1A _BV(0)
#define BP2D _BV(3)
#define BP2U _BV(2)
#define BP2A _BV(1)
#define BP3D _BV(1)
#define BP3U _BV(0)
#define BP3A _BV(2)

//a6 start signal / restartup signal
//a7 throttle

#define StartRpm 15000

#define CmdNextStep Step = NextStep[Step];
uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};
//power on
#define CmdPWROn  PORT6O = PWR_ON[Step];
//power off
#define CmdPWROff PORT6O = PWR_OFF[Step];

//�±۳���
uint8_t PWR_ON[] = {
	BP1U + BP1D + BP3D, // 1-2
	BP1U + BP1D + BP2D, // 1-3
	BP2U + BP1D + BP2D, // 2-3
	BP2U + BP3D + BP2D, // 2-1
	BP3U + BP3D + BP2D, // 3-1
	BP3U + BP1D + BP3D  // 3-2
};
uint8_t PWR_OFF[] = {
	0    + BP1D + BP3D, // 1-2
	0    + BP1D + BP2D, // 1-3
	0    + BP1D + BP2D, // 2-3
	0    + BP3D + BP2D, // 2-1
	0    + BP3D + BP2D, // 3-1
	0    + BP1D + BP3D  // 3-2
};

#define GetDigitRead (PIN3I & DigitRead[Step])
uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
#define GetDigitReadBaseVal (DigitReadBaseVal[Step])
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

uint8_t Step = 0;
uint8_t Status = 0;//0 halt ,1 running, 2 starting
uint8_t StartUpCount1=0;
volatile uint16_t TargetRPM=6000;//bit16 = start flg rest is data
volatile uint8_t FStart;
volatile uint16_t TempData;
volatile uint8_t TempDataCnt;
uint16_t rpm;
uint16_t startupCurrent;
uint16_t Power = 0;
uint16_t NextPower = 0;

void ClockInit();
void TimerInit();
void PCIntInit();
void loop();
void waita();
void adj();
void startup();


int main(void) {
	ClockInit();//��ʼ��ʱ�ӣ�1MHz -> 8MHz
	TimerInit();//��ʼ����ʱ�� 1/8
	PCIntInit();//��ʼ��ģ������
	TempDataCnt=16;
FStart=0;
	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input

	//�ȴ�1�������ź� Pin6�ߵ�ƽ  wait till 1s power on signal
	int cnt = 0;

	//������˿�
	DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
	
	//��ʼ������˿�
	PORT6O = BP1D | BP2D | BP3D;
	
	//��ʼ������˿�
	DDR3I = 0;
	PORT3I = 0;
	//��ѭ��
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

void TimerInit() {
	TCCR1A = 0;
	TCCR1B = 2;//  1/8	1MHz 1us
	TCCR1C = 0;
	TIMSK1 = 0;
}

void PCIntInit() {
  GIMSK |= _BV(PCIE0);
  PCMSK0 |= _BV(PCINT6);//CLK
  sei();
}

void loop() {
	for(;;) {
		//��ʱ������
		TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
		if(FStart)
		{
			CmdPWROn;
			uint16_t temp = TargetRPM>>1;
			for(;;)
			{
				if(currTick>=temp){
					CmdPWROff;
				}
				if(currTick>=TargetRPM){
					break;
				}
			}
			//ת�ٵ���
			adj();
		}
		else
		{
			Power = NextPower;
			if(Power)
			{
				CmdPWROn;
			}
			else
			{
				CmdPWROff;
			}
		
			if(Power<200 && rpm>500)
			{
				//�ȴ�����
				waita();
				//ת�ٵ���
				adj();
			}
			else
			{
				//ת�ٵ���
				adj();
				//�ȴ�����
				waita();
			}
		}
		CmdPWROff;
		CmdNextStep;
		//��¼��ǰת��
		rpm = currTick;
	}
}

void waita() {
	uint8_t valbase = GetDigitReadBaseVal;//;//;GetDigitRead
	uint16_t temp = (rpm>>1);
	for(;;)
	{
		if(currTick>Power){
			CmdPWROff;
		}
		
		if(FStart && currTick==65535)// power on signal
		{
			return;
		}
		
		uint8_t val = GetDigitRead;
		if(currTick>=temp && val!=valbase){
			return;
		}
	}
}

void adj() {
	NextPower=0;
	if(Status)
	{
		if(rpm>StartRpm)//too slow, halt
		{
			StartUpCount1 = 0;
			Status = 0;//halt
		}
		else if(rpm>TargetRPM)//bit slow && running
		{
			if(rpm>200)//��ת���½��о�ϸ����
			{
				uint32_t temp = TargetRPM;
				temp = temp << 16;//*65536
				temp = temp / rpm;
				temp = 65536 - temp;//65536~~>0 (0 Ϊת�ٷǳ��ӽ�)
				temp = rpm * temp;
				temp = temp >> 15;// ����Խ��Խ���    /1024
				NextPower = temp;
			}
			else
			{
				NextPower = 1000;
			}
		}
	}
	else
	{
		if(rpm<StartRpm&&rpm>(StartRpm>>3))//fast enough but not too fast
		{
			StartUpCount1++;
		}
		else
		{
			StartUpCount1 = 0;
		}
		if(StartUpCount1>40)
		{
			Status = 1;
		}
	}
}

ISR(PCINT0_vect){//���͸ߣ����͵�
  if(drCLK)//�����ض�ȡ
  {
    if(TempDataCnt==16)
    {
      if(drDAT)//��ʼλ
      {
        TempData = 0;
        TempDataCnt = 0;
      }
    }
    else
    {
      TempData<<=1;
      if(drDAT)
      {
        TempData|=1;
      }
      TempDataCnt++;
      if(TempDataCnt==16)
      {
        TargetRPM = TempData&0x7FFF;
		if(TempData&0x8000)
		{
		FStart=1;
		}
		else
		{
		FStart=0;
		}
      }
    }
  }
}
