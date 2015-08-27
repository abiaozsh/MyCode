#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define MaxPonTime 2000

#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

//drA3
#define drA7 (PINA & _BV(7)) /*analog*/
#define drA6 (PINA & _BV(6)) /*pwrOK*/

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

PROGMEM prog_uint16_t PWR_Table1[] = {50000,48000,46000,44000,42000,40000,38000,36000,34000,32000,30000,29000,28000,27000,26000,25000,24000,23000,22000,21000,20000,19000,18000,17000,16000,15000,14500,14000,13500,13000,12500,12000,11500,11000,10500,10000,9000,8000,7000,6000,5000,4000,3000,2000,1456,1424,1394,1365,1337,1310,1285,1260,1236,1213,1191,1170,1149,1130,1110,1092,1074,1057,1040,1024,1008,993,978,963,949,936,923,910,897,885,873,862,851,840,829,819,809,799,789,780,771,762,753,744,736,728,720,712,704,697,690,682,675,668,662,655,649,642,636,630,624,618,612,606,601,595,590,585,580,575,570,565,560,555,550,546,541,537,532,528,524,520,516,512,508,504,500,496,492,489,485,482,478,475,471,468,464,461,458,455,452,449,445,442,439,437,434,431,428,425,422,420,417,414,412,409,407,404,402,399,397,394,392,390,387,385,383,381,378,376,374,372,370,368,366,364,362,360,358,356,354,352,350,348,346,345,343,341,339,337,336,334,332,331,329,327,326,324,322,321,319,318,316,315,313,312,310,309,307,306,304,303,302,300,299,298,296,295,293,292,291,290,288,287,286,285,283,282,281,280,278,277,276,275,274,273,272,270,269,268,267,266,265,264,263,262,261,260,259,258,257,256};
PROGMEM prog_uint8_t PWR_Table2[] = {255,252,249,246,243,240,237,235,232,230,227,225,222,220,218,216,213,211,209,207,205,203,201,199,197,196,194,192,190,189,187,185,184,182,181,179,177,176,175,173,172,170,169,168,166,165,164,162,161,160,159,158,156,155,154,153,152,151,150,149,148,147,146,145,144,143,142,141,140,139,138,137,136,135,134,134,133,132,131,130,129,129,128,127,126,126,125,124,123,123,122,121,121,120,119,119,118,117,117,116,115,115,114,113,113,112,112,111,110,110,109,109,108,108,107,106,106,105,105,104,104,103,103,102,102,101,101,100,100,99,99,98,98,98,97,97,96,96,95,95,95,94,94,93,93,92,92,92,91,91,90,90,90,89,89,89,88,88,87,87,87,86,86,86,85,85,85,84,84,84,83,83,83,82,82,82,81,81,81,80,80,80,79,79,79,79,78,78,78,77,77,77,77,76,76,76,75,75,75,75,74,74,74,74,73,73,73,73,72,72,72,72,71,71,71,71,70,70,70,70,69,69,69,69,69,68,68,68,68,67,67,67,67,67,66,66,66,66,66,65,65,65,65,64,64,64,64,64,64,63,63,50,40,30,20,10};

uint8_t Step = 0;
uint8_t Status = 0;//0 halt ,1 running, 2 starting
uint8_t StartUpCount1=0;
uint16_t TargetRPM;
uint16_t rpm;
uint16_t startupCurrent;
uint16_t Power = 0;
uint16_t NextPower = 0;

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
	for(;;)
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
	ADCSRB = 0;//_BV(ADLAR);
	ADCSRA = _BV(ADEN) | _BV(ADSC) | _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2);
}

void loop() {
	for(;;) {
		//定时器清零
		TCNT1 = 0;TIFR1 |= _BV(TOV1);//timer reset //overflow flg reset
		if(drA6)
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
			//转速调整
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
				//等待过零
				waita();
				//转速调整
				adj();
			}
			else
			{
				//转速调整
				adj();
				//等待过零
				waita();
			}
		}
		CmdPWROff;
		CmdNextStep;
		//记录当前转速
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
		
		if(drA6 && currTick==65535)// power on signal
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
	NextPower = 0;
	uint16_t aread = ADC;
	sbi(ADCSRA, ADSC);
	if(aread>=512)
	{
		TargetRPM = pgm_read_byte_near(PWR_Table2 + ((aread&511)>>1));//高速表 8位
	}
	else
	{
		TargetRPM = pgm_read_word_near(PWR_Table1 + (aread>>1));//低速表 16位
	}
	if(Status)
	{
		if(rpm>StartRpm)//too slow, halt
		{
			StartUpCount1 = 0;
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
