#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "motor6.h"

//a6 start signal / restartup signal
//a7 throttle

#define HaltRpm  11000
#define StartRpm 12000

#define CmdNextStep Step = NextStep[Step];
uint8_t NextStep[] = {
  1,  2,  3,  4,  5,  0
};
//pwm on
#define CmdPWROn  PORT6O = PWR_ON[Step];
//pwm off
#define CmdPWROff PORT6O = PWR_OFF[Step];

//下臂常开
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
uint8_t DigitRead[] = {
  BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A 
};
#define GetDigitReadBaseVal (DigitReadBaseVal[Step])
uint8_t DigitReadBaseVal[] = {
  BP3A,     0,  BP1A,     0,  BP2A,     0
};

PROGMEM prog_uint16_t PWR_Table[] = {2500,2415,2335,2261,2191,2126,2064,2006,1951,1899,1849,1802,1758,1715,1675,1636,1600,1564,1531,1498,1467,1438,1409,1382,1355,1330,1306,1282,1259,1237,1216,1196,1176,1157,1138,1120,1103,1086,1070,1054,1038,1024,1009,995,981,968,955,942,930,918,906,895,883,873,862,852,842,832,822,813,804,795,786,777,769,760,752,745,737,729,722,715,707,700,694,687,680,674,668,661,655,649,643,638,632,626,621,615,610,605,600,595,590,585,580,576,571,566,562,557,553,549,545,540,536,532,528,525,521,517,513,509,506,502,499,495,492,488,485,482,479,475,472,469,466,463,460,457,454,451,448,445,443,440,437,435,432,429,427,424,422,419,417,414,412,409,407,405,403,400,398,396,394,391,389,387,385,383,381,379,377,375,373,371,369,367,365,363,361,360,358,356,354,353,351,349,347,346,344,342,341,339,337,336,334,333,331,330,328,327,325,324,322,321,319,318,316,315,314,312,311,309,308,307,305,304,303,302,300,299,298,296,295,294,293,292,290,289,288,287,286,285,283,282,281,280,279,278,277,276,275,274,273,271,270,269,268,267,266,265,264,263,262,261,261,260,259,258,257,256,255,254,253,252,251,250};

PROGMEM prog_uint16_t StartUp_Table[] = {49999,48120,46376,44755,43243,41830,40506,39263,38095,36994,35955,34972,34042,33160,32323,31527,30769,30046,29357,28699,28070,27467,26890,26337,25806,25296,24806,24334,23880,23443,23021,22614,22222,21843,21476,21122,20779,20447,20125,19814,19512,19219,18934,18658,18390,18130,17877,17630,17391,17158,16931,16710,16494,16284,16080,15880,15686,15496,15311,15130,14953,14780,14611,14446,14285,14128,13973,13822,13675,13530,13389,13250,13114,12981,12851,12723,12598,12475,12355,12237,12121,12007,11895,11786,11678,11573,11469,11367,11267,11169,11072,10977,10884,10792,10702,10613,10526,10440,10355,10272,10191,10110,10031,9953,9876,9800,9726,9653,9580,9509,9439,9370,9302,9235,9169,9103,9039,8976,8913,8852,8791,8731,8672,8613,8556,8499,8443,8387};

uint8_t Step = 0;
uint8_t Status = 0;//0 halt ,1 running, 2 starting
uint8_t StartUpCount1=0;
uint8_t StartUpCount2=0;
uint16_t rpm;
uint8_t startupIndex;
uint16_t startupCurrent;
uint16_t Power = 0;

void ClockInit();
void TimerInit();
void AnalogInit();
void wait(unsigned int ticks);
void loop();
void waita();
void adj();
void startup();


int main(void) {
	ClockInit();//初始化时钟：1MHz -> 8MHz
	TimerInit();//初始化定时器 1/8
	AnalogInit();//初始化模拟输入

	DDRA = 0;PORTA = 0;//all input
	DDRB = 0;PORTB = 0;//all input

	//等待1秒启动信号 Pin6高电平  wait till 1s power on signal
	int cnt = 0;
	while(true)
	{
		wait(1000);
		if(drA6==0)// power on signal
		{
			cnt=0;
		}
		cnt++;
		if(cnt>=1000)
		{
			break;
		}
	}
	while(drA6)
	{
	}

	//打开输出端口
	DDR6O = BP1U | BP1D | BP2U | BP2D | BP3U | BP3D;
	
	//初始化输出端口
	PORT6O = BP1D | BP2D | BP3D;
	
	//初始化输入端口
	DDR3I = 0;
	PORT3I = 0;

	//主循环
	loop();
}

void wait(unsigned int ticks) {
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

void AnalogInit() {
	ADMUX = 7;//A7 rpm
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
	ADCSRB = _BV(ADLAR);
}

void loop() {
	for(;;) {
		//定时器清零
		TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
		adj();
		CmdPWROn;
		//等待过零
		waita();
		CmdPWROff;
		CmdNextStep;
		//记录当前转速
		rpm = currTick;
	}
}

void waita() {
	if(Status == 2)
	{
		while(true)
		{
			//提早关闭 1/4 功率
			if(currTick>=Power){
				CmdPWROff;
			}
			if(currTick>=startupCurrent){
				return;
			}
		}
	}
	else
	{
		uint8_t valbase = GetDigitReadBaseVal;//;//;GetDigitRead
		uint16_t temp = (rpm>>1);//(rpm>>2)+40 or +80 //big for slow ,small for fast 
		while(true)
		{
			if(drA6)// power on signal
			{
				startup();
			}
			if(currTick>Power){
				CmdPWROff;
			}
			uint8_t val = GetDigitRead;
			if(currTick>=temp && val!=valbase){
				return;
			}
		}
	}
}

void adj() {
	Power = 0;
	uint8_t aread = ADCH;
	uint16_t TargetRPM = pgm_read_word_near(PWR_Table + aread);// TODO 512byte convert table  *32 8000max  ticks<<5
	sbi(ADCSRA, ADSC);
	if(Status == 1)
	{
		if(rpm>HaltRpm)//too slow, halt
		{
			StartUpCount1 = 0;
			StartUpCount2 = 0;
			Status = 0;//halt
		}
		else if(rpm>TargetRPM)//bit slow && running
		{
			uint32_t temp = TargetRPM;  // TargetRPM/rpm *65536  ->0~65536
			temp = temp << 16;//*65536
			temp = temp / rpm;
			temp = 65536 - temp;//65536~~>0 (0 为转速非常接近)
			temp = rpm * temp;
			temp = temp >> 10;//  /1024
			if(temp>rpm)
			{
				Power = rpm;
			}
			else
			{
				Power = temp;
			}
		}
	}
	else if(Status == 2)
	{
		startupIndex++;
		startupCurrent = pgm_read_word_near(StartUp_Table + startupIndex);
		Power = rpm>>2; //  1/4
		if(startupIndex == 127)
		{
			Status = 1;
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
			StartUpCount2 = 0;
		}
		if(StartUpCount1>20)
		{
			Power = 2000;
		}
		if(rpm<HaltRpm&&rpm>(HaltRpm>>3))//fast enough but not too fast
		{ 
			StartUpCount2++;
		}
		else
		{ 
			StartUpCount2=0;
		}
		if(StartUpCount2>20)
		{
			Status = 1;
		}
	}
}

void startup()
{
	//当前槽对准
	for(uint8_t i=0;i<255;i++)
	{
		CmdPWROn;
		wait(100);
		CmdPWROff;
		wait(1000);
	}
	startupIndex = 0;
	Status = 2;
}


