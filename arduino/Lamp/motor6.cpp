#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//�˿� A 0 1 2 3 ���� ��ʱֻ��0,2
//�˿� A 7 �չ�������
//�˿� B 0 �̵������(1)
//�˿� B 1 LED��� ��ʱ���� �����1(2)

//������4pin*2 ������*2 GND*1 LED*1
//������2pin*1 �չ��� GND ��������

//uint8_t NextStep[] = {  1,  2,  3,  4,  5,  0};

//PROGMEM prog_uint16_t PWR_Table[] = {8333};

//0 ���죬ȫ��
//1 ��䣬�ر�
//2 ��䣬��¥���ƣ��ȴ����߳� ����ʱ2���ӣ�
//3 ��䣬��¥���ƣ��ȴ����߳� ����ʱ2���ӣ�

uint8_t Status = 0;

void ClockInit();
void TimerInit();
uint8_t AnalogReadOn(uint8_t pin);
uint8_t AnalogReadOff(uint8_t pin);
void wait(unsigned int ticks);
void loop();


int main(void) {
	ClockInit();//��ʼ��ʱ�ӣ�1MHz -> 8MHz
	TimerInit();//��ʼ����ʱ�� 1/8

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input

	//�ȴ�1�������ź� Pin6�ߵ�ƽ  wait till 1s power on signal
	wait(1000);

	DDRA = 0;
	PORTA = 0;
	
	DDRB = 3;
	PORTB = 0;

	//��ѭ��
	loop();
}

void wait(unsigned int ticks) {
	TCCR1A = 0;
	TCCR1B = 2;//  1/8	1MHz 1us
	TCCR1C = 0;
	TIMSK1 = 0;
	TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
	while(currTick<ticks)
	{
		;
	}
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

uint8_t AnalogReadOn(uint8_t pin) {
	sbi(PORTB,1);
	wait(100);//0.1ms
	ADMUX = pin;
	ADCSRB = _BV(ADLAR);
	ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	sbi(ADCSRA, ADSC);
	while(ADCSRA & _BV(ADSC))
	{
	}
	uint8_t aread = ADCH;
	ADCSRA = 0;
	cbi(PORTB,1);
	return aread;
}
uint8_t AnalogReadOff(uint8_t pin) {
	wait(100);//0.1ms
	ADMUX = pin;
	ADCSRB = _BV(ADLAR);
	ADCSRA = _BV(ADEN) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	sbi(ADCSRA, ADSC);
	while(ADCSRA & _BV(ADSC))
	{
	}
	uint8_t aread = ADCH;
	ADCSRA = 0;
	return aread;
}

void loop() {
	for(;;) {
		//���ڰ��죬����Ƿ�������
		if(Status==0)
		{
			uint8_t val = AnalogReadOff(7);//�չ�������
			if(val<64)
			{
				Status = 1;
			}
			wait(1000);
		}
		//��������
		if(Status==1)
		{
			uint8_t val;
			uint8_t valBase;
			
			val = AnalogReadOff(7);//�չ�������
			if(val>64)
			{
				Status = 0;
			}
			
			valBase = AnalogReadOff(0);//ֵС
			val = AnalogReadOn(0);//ֵ��
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 2;
				sbi(PORTB,0);
				for(;;) {
					valBase = AnalogReadOff(0);//ֵС
					val = AnalogReadOn(0);//ֵ��
					if((val>valBase) && ((val-valBase)>40))
					{
						break;
					}
				}
				continue;
			}
			
			
			valBase = AnalogReadOff(2);
			val = AnalogReadOn(2);
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 3;
				sbi(PORTB,0);
				for(;;) {
					valBase = AnalogReadOff(2);//ֵС
					val = AnalogReadOn(2);//ֵ��
					if((val>valBase) && ((val-valBase)>40))
					{
						break;
					}
				}
				continue;
			}
			
			wait(1000);//wait 1ms
		}
		if(Status==2)
		{
			uint8_t val;
			uint8_t valBase;
			valBase = AnalogReadOff(2);//ֵС
			val = AnalogReadOn(2);//ֵ��
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 1;
				cbi(PORTB,0);
				wait(60000);
				for(;;) {
					valBase = AnalogReadOff(2);//ֵС
					val = AnalogReadOn(2);//ֵ��
					if((val>valBase) && ((val-valBase)>40))
					{
						break;
					}
				}
			}
		}
		if(Status==3)
		{
			uint8_t val;
			uint8_t valBase;
			valBase = AnalogReadOff(0);//ֵС
			val = AnalogReadOn(0);//ֵ��
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 1;
				cbi(PORTB,0);
				wait(60000);
				for(;;) {
					valBase = AnalogReadOff(0);//ֵС
					val = AnalogReadOn(0);//ֵ��
					if((val>valBase) && ((val-valBase)>40))
					{
						break;
					}
				}
			}
		}


	}
}
