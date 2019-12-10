#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define CUR_TIMING_REC TIMINGR_8M_TCCR1B_1___9600
#define CUR_TIMING TIMING__8M_TCCR1B_1___9600
//#define CUR_TIMING TIMING__8M_TCCR1B_1_115200
#define TCCR1B_Value 1

PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1_115200[] = {   69,  138,  208,  277,  347,  416,  486,  555,  625,  694};
PROGMEM prog_uint16_t TIMING__8M_TCCR1B_1___9600[] = {  833, 1666, 2500, 3333, 4166, 5000, 5833, 6666, 7500, 8333};
PROGMEM prog_uint16_t TIMINGR_8M_TCCR1B_1___9600[] = { 1233, 2066, 2900, 3733, 4566, 5400, 6233, 7066, 7900, 8333};

//pd0 rxd		usb txd
//pd1 txd		usb rxd
#define DDR_Send DDRD
#define PORT_Send PORTD
#define BIT_Send _BV(1)
#define DDR_Recv DDRD
#define PIN_Recv PIND
#define BIT_Recv _BV(0)

void SerialInit(){
	//UCSR0B = 0;//not forget turnoff usart0 on mega328p
	DDR_Send |= BIT_Send;
	DDR_Recv &= ~BIT_Recv;
	PORT_Send |= BIT_Send;
}
void TimerInit()
{
  // COM1A1 COM1A0 COM1B1 COM1B0 ¨C ¨C WGM11 WGM10
  TCCR1A = 0;
  //ICNC1 ICES1 ¨C WGM13 WGM12 CS12 CS11 CS10

  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)
  //TCCR1B = 1;//  1/1
  //TCCR1B = 2;//  1/8 //1Mhz
  //TCCR1B = 3;//  1/64
  
  TCCR1B = TCCR1B_Value;
}
void SerialSend(uint8_t val){
	cli();
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING;
	PORT_Send &= ~BIT_Send;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);//startbit
	uint8_t chkbit = 0x01;
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		if(val&chkbit){PORT_Send |= BIT_Send;}else{PORT_Send &= ~BIT_Send;}chkbit<<=1;timing = pgm_read_word_near(pTiming++);while(TCNT1<timing);
	}
	PORT_Send |= BIT_Send;timing = pgm_read_word_near(pTiming);while(TCNT1<timing);
	sei();
}

uint8_t SerialRead()
{
	cli();
	uint8_t val = 0;
	//TCCR1B = *timoutParam;//timeout param 1~5 : 1/(1~1024) max (0.008192 s ~ 8.388608 s)
	//TCNT1 = 0;TIFR |= _BV(TOV1);
	while(PIN_Recv&BIT_Recv){
		//if(((TIFR & _BV(TOV1))?0x0FFFF:TCNT1)>=timeout)
		//{
		//	*timoutParam = 0;
		//	return 0;
		//}
	}
	TCCR1B = TCCR1B_Value;
	TCNT1 = 0;
	uint16_t timing;
	prog_uint16_t* pTiming = CUR_TIMING_REC;
	timing = pgm_read_word_near(pTiming++);
	while(TCNT1<timing);
	
	for(uint8_t i = 8 ; i > 0 ; i--)
	{
		val>>=1;val |= (PIN_Recv&BIT_Recv?0x80:0);
		timing = pgm_read_word_near(pTiming++);
		while(TCNT1<timing);
	}
	sei();
	return val;
}

void wait(uint16_t ticks)
{
	TCNT1 = 0;//timer reset
	while(TCNT1<(ticks<<8))
	{
		;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

char convt[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void printOK(){
  SerialSend('O');SerialSend('K');
}
void printHex(uint16_t val){//"ll"+"hh"
  SerialSend(convt[((val & 0xF0) >> 4)]);
  SerialSend(convt[((val & 0x0F))]);
  SerialSend(convt[((val & 0xF000) >> 12)]);
  SerialSend(convt[((val & 0x0F00) >> 8)]);
}
void printHex(uint8_t val){//"hl"
  SerialSend(convt[((val & 0xF0) >> 4)]);
  SerialSend(convt[((val & 0x0F))]);
}

uint8_t ConvBCD(uint8_t val){
  if(val>='0'&&val<='9')
  {
    val = val - '0';
  }
  else if(val>='a'&&val<='f')
  {
    val = val - 'a' + 10;
  }
  else if(val>='A'&&val<='F')
  {
    val = val - 'A' + 10;
  }
  return val;
}

uint8_t GetByte(){
  uint8_t vh = SerialRead();
  uint8_t vl = SerialRead();
  uint8_t val = (( ConvBCD(vh) << 4 ) & 0xF0 ) | (ConvBCD(vl) & 0x0F);
  return val;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
atmega8p
8io pb data
pb0		PB0
pb1		PB1
pb2		PB2
pb3		PB3
pb4		PB4
pb5		PB5
pb6		PC0
pb7		PC1

pc0 in  rdy	PD1
pc1 out clock	PB6
pc2 out bs2	PC2
pc3 out VCC 5v
pc4 out V12 12v

pd0 rxd		usb txd
pd1 txd		usb rxd

pd2 out !oe	PD2
pd3 out !wr	PD3
pd4 out bs1	PD4
pd5 out xa0	PD5
pd6 out xa1	PD6
pd7 out pagel	PD7
*/
#define DATA      PORTB
#define DATAPIN   PINB
#define DATA_IN   DDRB = 0
#define DATA_OUT  DDRB = 0xFF

#define RDY  PINC & _BV(0)

#define CLK_OUT  DDRC  |=  _BV(1)
#define BS2_OUT  DDRC  |=  _BV(2)
#define VCC_OUT  DDRC  |=  _BV(3)
#define V12_OUT  DDRC  |=  _BV(4)
#define O_E_OUT  DDRD  |=  _BV(2)
#define W_R_OUT  DDRD  |=  _BV(3)
#define BS1_OUT  DDRD  |=  _BV(4)
#define XA0_OUT  DDRD  |=  _BV(5)
#define XA1_OUT  DDRD  |=  _BV(6)
#define PGL_OUT  DDRD  |=  _BV(7)

#define CLK_IN  DDRC  &=  ~_BV(1)
#define BS2_IN  DDRC  &=  ~_BV(2)
#define VCC_IN  DDRC  &=  ~_BV(3)
#define V12_IN  DDRC  &=  ~_BV(4)
#define O_E_IN  DDRD  &=  ~_BV(2)
#define W_R_IN  DDRD  &=  ~_BV(3)
#define BS1_IN  DDRD  &=  ~_BV(4)
#define XA0_IN  DDRD  &=  ~_BV(5)
#define XA1_IN  DDRD  &=  ~_BV(6)
#define PGL_IN  DDRD  &=  ~_BV(7)

#define CLK_H    PORTC |=  _BV(1)
#define CLK_L    PORTC &= ~_BV(1)

#define BS2_H    PORTC |=  _BV(2)
#define BS2_L    PORTC &= ~_BV(2)

#define VCC_ON   PORTC &= ~_BV(3)
#define VCC_OFF  PORTC |=  _BV(3)

#define V12_ON   PORTC |=  _BV(4)
#define V12_OFF  PORTC &= ~_BV(4)

#define O_E_H    PORTD |=  _BV(2)
#define O_E_L    PORTD &= ~_BV(2)

#define W_R_H    PORTD |=  _BV(3)
#define W_R_L    PORTD &= ~_BV(3)

#define BS1_H    PORTD |=  _BV(4)
#define BS1_L    PORTD &= ~_BV(4)

#define XA0_H    PORTD |=  _BV(5)
#define XA0_L    PORTD &= ~_BV(5)

#define XA1_H    PORTD |=  _BV(6)
#define XA1_L    PORTD &= ~_BV(6)

#define PGL_H    PORTD |=  _BV(7)
#define PGL_L    PORTD &= ~_BV(7)

void waitt(uint16_t time){
	TCNT1 = 0;//timer reset
	while(TCNT1<(time))
	{
		;
	}
}

void wait2()
{
	TCNT1 = 0;//timer reset
	while(TCNT1<(10000))
	{
		;
	}
}

void wait()
{
	for(uint8_t i=0;i<2;i++){
		wait2();
	}
}

//ok
void loadCommand(uint8_t command){
    //A. Load Command ¡°Write Flash¡±
    //1. Set XA1, XA0 to ¡°10¡±. This enables command loading
    //2. Set BS1 to ¡±0¡±
    //3. Set DATA to ¡°0001 0000¡±. This is the command for Write Flash
    //4. Give XTAL1 a positive pulse. This loads the command
    DATA_OUT;
    XA0_L;
    XA1_H;
    waitt(100);
	
    BS1_L;
    BS2_L;
    waitt(100);
    DATA = command;
	
			
    waitt(100);
    CLK_H;
    waitt(100);
    CLK_L;
    DATA_IN;
}


void loadAddressLow(uint8_t address){
    //B. Load Address Low byte
    //1. Set XA1, XA0 to ¡°00¡±. This enables address loading
    //2. Set BS1 to ¡°0¡±. This selects low address
    //3. Set DATA = Address Low byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the address Low byte
    DATA_OUT;
    BS1_L;
    BS2_L;
    DATA = address;
    XA0_L;
    XA1_L;
    waitt(100);
    CLK_H;
    waitt(100);
    CLK_L;
    DATA_IN;
}
void loadAddressHigh(uint8_t address){
    //G. Load Address High byte
    //1. Set XA1, XA0 to ¡°00¡±. This enables address loading
    //2. Set BS1 to ¡°1¡±. This selects high address
    //3. Set DATA = Address High byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the address High byte
    DATA_OUT;
    BS1_H;
    BS2_L;
    DATA = address;
    XA0_L;
    XA1_L;
    waitt(100);
    CLK_H;
    waitt(100);
    CLK_L;
    DATA_IN;
}

void loadDataLow(uint8_t data){
    //C. Load Data Low byte
    //1. Set XA1, XA0 to ¡°01¡±. This enables data loading
    //2. Set DATA = Data Low byte (0x00 - 0xFF)
    //3. Give XTAL1 a positive pulse. This loads the data byte
    DATA_OUT;
    BS1_L;
    BS2_L;
    waitt(100);
    XA0_H;
    XA1_L;
    waitt(100);
    DATA = data;
    waitt(100);
    CLK_H;
    waitt(100);
    CLK_L;
    DATA_IN;
}

void loadDataHigh(uint8_t data){
    //D. Load Data High byte
    //1. Set BS1 to ¡°1¡±. This selects high data byte
    //2. Set XA1, XA0 to ¡°01¡±. This enables data loading
    //3. Set DATA = Data High byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the data byte
    DATA_OUT;
    BS1_H;
    BS2_L;
    waitt(100);
    XA0_H;
    XA1_L;
    waitt(100);
    DATA = data;
    waitt(100);
    CLK_H;
    waitt(100);
    CLK_L;
    DATA_IN;
}

//BS1_  BS2_
//Reading the Flash The algorithm for reading the Flash memory is as follows (refer to ¡°Programming the Flash¡± on
//page 222 for details on Command and Address loading):
//1. A: Load Command ¡°0000 0010¡±
//2. G: Load Address High byte (0x00 - 0xFF)
//3. B: Load Address Low byte (0x00 - 0xFF)
//4. Set OE to ¡°0¡±, and BS1 to ¡°0¡±. The Flash word Low byte can now be read at DATA
//5. Set BS1 to ¡°1¡±. The Flash word High byte can now be read at DATA
//6. Set OE to ¡°1¡±
    //uint8_t data;
    //O_E_L;
    //BS1_L;
    //BS2_L;
    //XA0_H;
    //XA1_H;
    //wait();
    //CLK_H;
    //wait();
    //CLK_L;
    //wait();
    //data = DATA;
    //O_E_H;
    //printHex(data);
    

void readData00(){
    O_E_L;
    BS1_L;
    BS2_L;
	
	
	
    waitt(100);
    //XA0_H;
    //XA1_H;
    //wait();
    //CLK_H;
    //wait();
    //CLK_L;
    //wait();
	
    uint8_t data = DATAPIN;
    O_E_H;
    printHex(data);
}
    

void readData11(){
    O_E_L;
    BS1_H;
    BS2_H;
	
	
    waitt(100);
    //XA0_H;
    //XA1_H;
    //wait();
    //CLK_H;
    //wait();
    //CLK_L;
    //wait();
	
	
    uint8_t data = DATAPIN;
    O_E_H;
    printHex(data);
}
void readData10(){
    O_E_L;
    BS1_H;
    BS2_L;
    
	
	
	
	waitt(100);
	//XA0_H;
    //XA1_H;
    //wait();
    //CLK_H;
    //wait();
    //CLK_L;
    //wait();

	
	
	
    uint8_t data = DATAPIN;
    O_E_H;
    printHex(data);
}

void doWrite(){
    W_R_L;
    waitt(1000);
    W_R_H;
    waitt(1000);
    //Wait until RDY/BSY goes high
    while(!(RDY));
}

void powerON(){
      //VCC_ON;
      //V12_ON;
      //
	  //DATA_IN;
    
    BS2_L;
	BS2_OUT;
	CLK_L;
	CLK_OUT;
    O_E_H;
	O_E_OUT;
    W_R_H;
	W_R_OUT;
    wait();
	
    PGL_L;
	PGL_OUT;
    XA1_L;
	XA1_OUT;
    XA0_L;
	XA0_OUT;
    BS1_L;
	BS1_OUT;
    wait();
	
    //2. Apply 4.5V - 5.5V between VCC and GND simultaneously as 11.5V - 12.5V is applied to RESET
    //port.Write("x");// vcc * v12 on
    VCC_ON;
    V12_ON;

    //3. Wait 100ns
    wait();
	
}

void powerOFF(){
	DATA_IN;
	PGL_IN;
	XA0_IN;
	XA1_IN;
	BS1_IN;
	BS2_IN;
	CLK_IN;
	O_E_IN;
	W_R_IN;
	
	
    VCC_OFF;
    V12_OFF;
}

//read fUse
//strued

//write default fUse
//stwued





uint8_t bufferL[32];
uint8_t bufferH[32];

void loop()
{
  uint8_t cmd1 = SerialRead();
  if(cmd1>='a'&&cmd1<='z'){}else{return;}
  uint8_t cmd2 = SerialRead();
  if(cmd2>='a'&&cmd2<='z'){}else{return;}

       if(cmd1=='e' && cmd2=='d')       //ed End
  {
    powerOFF();
    printOK();
  }
  else if(cmd1=='s' && cmd2=='t')       //st Start
  {
    powerON();
    printOK();
  }
  else if(cmd1=='s' && cmd2=='i')       //si ReadSignatureBytes
  {
    //1. A: Load Command ??0000 1000??
    loadCommand(0x08);
    //2. B: Load Address Low byte (0x00 - 0x02)
    loadAddressLow(0x00);
    //3. Set OE to ??0??, and BS1 to ??0??. The selected Signature byte can now be read at DATA.
    readData00();
    loadAddressLow(0x01);
    readData00();
    loadAddressLow(0x02);
    readData00();
  }
  else if(cmd1=='e' && cmd2=='r')       //er Erase
  {
    loadCommand(0x80);
    //5. Give WR a negative pulse. This starts the Chip Erase. RDY/BSY goes low
    doWrite();
    printOK();
  }
  else if(cmd1=='r' && cmd2=='u')       //rf read fUse
  {
    //1. A: Load Command ??0000 0100??
    loadCommand(0x04);
    //2. Set OE to ??0??, BS2 to ??0??, and BS1 to ??0??. The status of the Fuse Low bits can now be read at DATA (??0?? means programmed)
    readData00();
    //3. Set OE to ??0??, BS2 to ??1??, and BS1 to ??1??. The status of the Fuse High bits can now be read at DATA (??0?? means programmed)
    readData11();
    //4. Set OE to ??0??, BS2 to ??0??, and BS1 to ??1??. The status of the Lock Bits can now be read at DATA (??0?? means programmed)
    readData10();
    //5. Set OE to ??1??
  }
  else if(cmd1=='w' && cmd2=='u')       //wf write default fUse
  {
    //A: Load Command ??0100 0000??
    loadCommand(0x40);
    //2. C: Load Data Low byte. Bit n = ??0?? programs and bit n = ??1?? erases the Fuse bit
    //3. Set BS1 and BS2 to ??0??
    loadDataLow(0xE4);//0xE4 1110 0100  8Mhz
    //4. Give WR a negative pulse and wait for RDY/BSY to go high
    doWrite();

    //1. A: Load Command ??0100 0000??
    //loadCommand(0x40);
    //2. C: Load Data Low byte. Bit n = ??0?? programs and bit n = ??1?? erases the Fuse bit
    loadDataLow(0xF9);//0xF9 1111 1001 
    //3. Set BS1 to ??1?? and BS2 to ??0??. This selects high data byte
    BS1_H;
    BS2_L;
    //4. Give WR a negative pulse and wait for RDY/BSY to go high
    //5. Set BS1 to ??0??. This selects low data byte
    doWrite();
    BS1_L;
    printOK();
  }
  else if(cmd1=='w' && cmd2=='f')       //wf write flash
  {
    //A. Load Command ¡°Write Flash¡±
    //1. Set XA1, XA0 to ¡°10¡±. This enables command loading
    //2. Set BS1 to ¡±0¡±
    //3. Set DATA to ¡°0001 0000¡±. This is the command for Write Flash
    //4. Give XTAL1 a positive pulse. This loads the command
    loadCommand(0x10);
    printOK();
  }
  else if(cmd1=='p' && cmd2=='b')       //pb LoadFlashPageBuffer
  {
    uint8_t valal = GetByte();
    for(uint8_t i = 0;i<32;i++){
      bufferL[i] = GetByte();
      bufferH[i] = GetByte();
    }

    for(uint8_t i = 0;i<32;i++){
      uint8_t valdl = bufferL[i];
      uint8_t valdh = bufferH[i];
      //B. Load Address Low byte
      //1. Set XA1, XA0 to ¡°00¡±. This enables address loading
      //2. Set BS1 to ¡°0¡±. This selects low address
      //3. Set DATA = Address Low byte (0x00 - 0xFF)
      //4. Give XTAL1 a positive pulse. This loads the address Low byte
      loadAddressLow(valal);
      valal++;
      //C. Load Data Low byte
      //1. Set XA1, XA0 to ¡°01¡±. This enables data loading
      //2. Set DATA = Data Low byte (0x00 - 0xFF)
      //3. Give XTAL1 a positive pulse. This loads the data byte
      loadDataLow(valdl);
      //D. Load Data High byte
      //1. Set BS1 to ¡°1¡±. This selects high data byte
      //2. Set XA1, XA0 to ¡°01¡±. This enables data loading
      //3. Set DATA = Data High byte (0x00 - 0xFF)
      //4. Give XTAL1 a positive pulse. This loads the data byte
      loadDataHigh(valdh);
      //E. Latch Data
      //1. Set BS1 to ¡°1¡±. This selects high data byte
      //2. Give PAGEL a positive pulse. This latches the data bytes (see Figure 106 on page 224 for signal waveforms)    
      BS1_H;
      waitt(100);
      PGL_H;
      waitt(100);
      PGL_L;
      waitt(100);
    }
    printOK();
  }
  else if(cmd1=='h' && cmd2=='a')       //ha LoadFlashHighAddress
  {
    uint8_t valah = GetByte();
    //G. Load Address High byte
    //1. Set XA1, XA0 to ¡°00¡±. This enables address loading
    //2. Set BS1 to ¡°1¡±. This selects high address
    //3. Set DATA = Address High byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the address High byte
    loadAddressHigh(valah);
    
    //H. Program Page
    //1. Set BS1 = ¡°0¡±
    BS1_L;
    //2. Give WR a negative pulse. This starts programming of the entire page of data. RDY/BSY
    //goes low
    //3. Wait until RDY/BSY goes high. (See Figure 106 on page 224 for signal waveforms)
    doWrite();
    
    printOK();
  }
  else if(cmd1=='n' && cmd2=='o')       //no NOP
  {
    //J. End Page Programming
    //1. Set XA1, XA0 to ¡°10¡±. This enables command loading
    //2. Set DATA to ¡°0000 0000¡±. This is the command for No Operation
    //3. Give XTAL1 a positive pulse. This loads the command, and the internal write signals are reset  
    loadCommand(0x00);
    printOK();
  }
  else if(cmd1=='r' && cmd2=='f')       //rf ReadFlash
  {
    //1. A: Load Command ¡°0000 0010¡±
    loadCommand(0x02);
    printOK();
  }
  else if(cmd1=='f' && cmd2=='b')       //fb ReadFlashLowAndHighBytes1
  {
    uint8_t valal = GetByte();
    uint8_t valah = GetByte();
    //2. G: Load Address High byte (0x00 - 0xFF)
    loadAddressHigh(valah);
    //3. B: Load Address Low byte (0x00 - 0xFF)
    loadAddressLow(valal);
    //4. Set OE to ¡°0¡±, and BS1 to ¡°0¡±. The Flash word Low byte can now be read at DATA
    readData00();
    //5. Set BS1 to ¡°1¡±. The Flash word High byte can now be read at DATA
    readData10();
    //6. Set OE to ¡°1¡±
  }
}

int main()
{
	TimerInit(); //do later
	SerialInit();
    VCC_OFF;
    V12_OFF;
	VCC_OUT;
	V12_OUT;
	
  //DATA_IN  ;
  //CLK_L    ;
  //CLK_OUT  ;
  //BS2_L    ;
  //BS2_OUT  ;
  //VCC_OFF  ;
  //VCC_OUT  ;
  //V12_OFF  ;
  //V12_OUT  ;
  //O_E_H    ;
  //O_E_OUT  ;
  //W_R_H    ;
  //W_R_OUT  ;
  //BS1_L    ;
  //BS1_OUT  ;
  //XA0_L    ;
  //XA0_OUT  ;
  //XA1_L    ;
  //XA1_OUT  ;
  //PGL_L    ;
  //PGL_OUT  ;


	for (;;) {
    loop();
	}
        
	return 0;
}
