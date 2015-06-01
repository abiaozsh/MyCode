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
//power on
#define CmdPWROn  PORT6O = PWR_ON[Step];
//power off
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
uint8_t DigitRead[] =        {BP3A,  BP2A,  BP1A,  BP3A,  BP2A,  BP1A};
#define GetDigitReadBaseVal (DigitReadBaseVal[Step])
uint8_t DigitReadBaseVal[] = {BP3A,     0,  BP1A,     0,  BP2A,     0};

PROGMEM prog_uint16_t PWR_Table[] = {8333,6632,5507,4709,4113,3650,3282,2980,2730,2518,2337,2180,2043,1922,1815,1719,1632,1554,1483,1418,1359,1304,1254,1207,1164,1124,1086,
1051,1018,987,958,930,904,880,857,835,814,794,775,757,740,723,707,692,678,664,651,638,625,614,602,591,581,570,561,551,542,533,524,516,508,500,492,485,478,471,464,458,451,445,
439,433,428,422,417,411,406,401,396,391,387,382,378,373,369,365,361,357,353,349,345,342,338,335,331,328,325,321,318,315,312,309,306,303,301,298,295,292,290,287,285,282,280,
277,275,273,270,268,266,264,262,260,258,256,254,252,250,248,246,244,242,240,239,237,235,233,232,230,228,227,225,224,222,221,219,218,216,215,213,212,211,209,208,207,205,204,
203,201,200,199,198,197,195,194,193,192,191,190,188,187,186,185,184,183,182,181,180,179,178,177,176,175,174,173,172,171,171,170,169,168,167,166,165,164,164,163,162,161,160,
160,159,158,157,157,156,155,154,154,153,152,151,151,150,149,149,148,147,147,146,145,145,144,143,143,142,141,141,140,140,139,138,138,137,137,136,135,135,134,134,133,133,132,
132,131,131,130,130,129,128,128,127,120,100,80,60,40};

PROGMEM prog_uint16_t StartUp_Table[] = {64102,57626,52420,48145,44571,41540,38938,36679,34700,32952,31398,30006,28753,27620,26590,25649,24787,23995,23264,22587,21959,21375,
20831,20322,19845,19398,18977,18582,18208,17856,17522,17206,16906,16622,16351,16093,15848,15614,15391,15177,14973,14778,14590,14411,14239,14074,13915,13762,13616,13474,13338,
13207,13081,12959,12842,12728,12618,12512,12410,12311,12215,12122,12032,11945,11861,11779,11700,11623,11548,11476,11406,11337,11271,11207,11144,11083,11024,10966,10910,10856,
10803,10751,10701,10652,10604,10558,10512,10468,10425,10383,10342,10302,10263,10225,10188,10152,10117,10082,10049,10016,9984,9952,9922,9892,9862,9834,9806,9779,9752,9726,
9700,9675,9651,9627,9604,9581,9558,9537,9515,9494,9474,9454,9434,9415,9396,9378,9360,9342};

uint8_t Step = 0;
uint8_t Status = 0;//0 halt ,1 running, 2 starting
uint8_t StartUpCount1=0;
uint8_t StartUpCount2=0;
uint8_t startupIndex;
uint16_t rpm;
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
		if(Power)
		{
			CmdPWROn;
		}
		else
		{
			CmdPWROff;
		}
		adj();
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
		uint16_t temp = (rpm>>1);
		while(true)
		{
			if(drA6)// power on signal
			{
				CmdPWROff;
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
	sbi(ADCSRA, ADSC);
	uint16_t TargetRPM = pgm_read_word_near(PWR_Table + aread);// TODO 512byte convert table  *32 8000max  ticks<<5
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
			if(rpm>200)//低转速下进行精细运算
			{
				uint32_t temp = TargetRPM;  // TargetRPM/rpm *65536  ->0~65536
				temp = temp << 16;//*65536
				temp = temp / rpm;
				temp = 65536 - temp;//65536~~>0 (0 为转速非常接近)
				temp = rpm * temp;
				temp = temp >> 15;// 数字越大越柔和    /1024
				Power = temp;
			}
			else
			{
				Power = 1000;
			}
		}
	}
	else if(Status == 2)
	{
		startupIndex++;
		startupCurrent = pgm_read_word_near(StartUp_Table + startupIndex);
		uint32_t temp = aread;
		temp = temp * rpm;// rpm * n / 256; n = (0~255)
		temp = temp >> 8;
		Power = temp + 20;
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
			Power = rpm>>2;
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
	while(drA6)
	{
	}
	while(!drA6)
	{
	}
	//当前槽对准
	for(uint16_t i=0;i<2550;i++)s
	{
		uint8_t aread = ADCH;
		sbi(ADCSRA, ADSC);
		CmdPWROn;
		wait(aread);
		CmdPWROff;
		wait(1000-aread);
	}
	startupIndex = 0;
	Status = 2;
}


