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

//端口 A 0 1 2 3 输入 暂时只用0,2
//端口 A 7 日光检测输入
//端口 B 0 继电器输出(1)
//端口 B 1 LED输出 暂时不用 恒输出1(2)

//插座：4pin*2 传感器*2 GND*1 LED*1
//插座：2pin*1 日光检测 GND 光敏电阻

//uint8_t NextStep[] = {  1,  2,  3,  4,  5,  0};

//PROGMEM prog_uint16_t PWR_Table[] = {8333};

//0 白天，全关
//1 晚间，关闭
//2 晚间，上楼开灯，等待人走出 （超时2分钟）
//3 晚间，下楼开灯，等待人走出 （超时2分钟）

uint8_t Status = 0;

void ClockInit();
void TimerInit();
uint8_t AnalogReadOn(uint8_t pin);
uint8_t AnalogReadOff(uint8_t pin);
void wait(unsigned int ticks);
void loop();


int main(void) {
	ClockInit();//初始化时钟：1MHz -> 8MHz
	TimerInit();//初始化定时器 1/8

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input

	//等待1秒启动信号 Pin6高电平  wait till 1s power on signal
	wait(1000);

	DDRA = 0;
	PORTA = 0;
	
	DDRB = 3;
	PORTB = 0;

	//主循环
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
		//正在白天，检测是否到晚上了
		if(Status==0)
		{
			uint8_t val = AnalogReadOff(7);//日光检测输入
			if(val<64)
			{
				Status = 1;
			}
			wait(1000);
		}
		//正在晚上
		if(Status==1)
		{
			uint8_t val;
			uint8_t valBase;
			
			val = AnalogReadOff(7);//日光检测输入
			if(val>64)
			{
				Status = 0;
			}
			
			valBase = AnalogReadOff(0);//值小
			val = AnalogReadOn(0);//值大
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 2;
				sbi(PORTB,0);
				for(;;) {
					valBase = AnalogReadOff(0);//值小
					val = AnalogReadOn(0);//值大
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
					valBase = AnalogReadOff(2);//值小
					val = AnalogReadOn(2);//值大
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
			valBase = AnalogReadOff(2);//值小
			val = AnalogReadOn(2);//值大
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 1;
				cbi(PORTB,0);
				wait(60000);
				for(;;) {
					valBase = AnalogReadOff(2);//值小
					val = AnalogReadOn(2);//值大
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
			valBase = AnalogReadOff(0);//值小
			val = AnalogReadOn(0);//值大
			if(!((val>valBase) && ((val-valBase)>40)))
			{
				Status = 1;
				cbi(PORTB,0);
				wait(60000);
				for(;;) {
					valBase = AnalogReadOff(0);//值小
					val = AnalogReadOn(0);//值大
					if((val>valBase) && ((val-valBase)>40))
					{
						break;
					}
				}
			}
		}


	}
}
