//INPUTS
//A (Pin 14)
//Serial Data Input. The data on this pin is shifted into the
//8–bit serial shift register.
//CONTROL INPUTS
//Shift Clock (Pin 11)
//Shift Register Clock Input. A low– to–high transition on
//this input causes the data at the Serial Input pin to be shifted
//into the 8–bit shift register.
//Reset (Pin 10)
//Active–low, Asynchronous, Shift Register Reset Input. A
//low on this pin resets the shift register portion of this device
//only. The 8–bit latch is not affected.
//Latch Clock (Pin 12)
//Storage Latch Clock Input. A low–to–high transition on
//this input latches the shift register data.
//Output Enable (Pin 13)
//Active–low Output Enable. A low on this input allows the
//data from the latches to be presented at the outputs. A high
//on this input forces the outputs (QA–QH) into the
//high–impedance state. The serial output is not affected by
//this control unit.
//OUTPUTS
//QA – QH (Pins 15, 1, 2, 3, 4, 5, 6, 7)
//Noninverted, 3–state, latch outputs.
//SQH (Pin 9)
//Noninverted, Serial Data Output. This is the output of the
//eighth stage of the 8–bit shift register. This output does not
//have three–state capability.

//符号　　	引脚　　	描述　　
//Q0--Q7　　	第15脚，第1-7脚	8位并行数据输出，　　
//GND　　	第8脚　　	地　　
//Q7’　　	第9脚　　	串行数据输出　　
//MR　　	第10脚　　	主复位（低电平）　　
//SHCP　　	第11脚　　	移位寄存器时钟输入　　
//STCP　　	第12脚　　	存储寄存器时钟输入　　
//OE　　	第13脚　　	输出有效（低电平）　　
//DS　　	第14脚　　	串行数据输入　　
//VCC　　	第16脚　　	电源　　


//33,127,10,10000,

void setup()
{
	DDRB |= _BV(1);//pin9 B1 Shift Clock Pin 11
	DDRB |= _BV(0);//pin8 B0 Store Clock Pin 12
	DDRD |= _BV(5);//pin5 D5 Output Enable Pin 13
	DDRD |= _BV(4);//pin4 D4 Serial Data Input Pin 14
	DDRD |= _BV(3);//PNP Drive
	
	PORTD &= ~_BV(3);//PNP disable L
	PORTD |= _BV(5);//OE disable H
	
	Serial.begin(9600);
	uint8_t data;
	uint16_t time;
	uint16_t total;
	while(true)
	{
		int start = Serial.parseInt();
		if(start == 33)
		{
			data = Serial.parseInt();
			time = Serial.parseInt();
			total = Serial.parseInt();
			break;
		}
	}
	
	PORTB &= ~(_BV(1) | _BV(0)); //clear clock
	delay(10);
	for(uint8_t i=0;i<8;i++)
	{
		if(data & 1){PORTD |= _BV(4);}else{PORTD &= ~_BV(4);}//set data
		data = data >> 1;
		delay(10);
		PORTB |= _BV(1); //shift clock up
		delay(10);
		PORTB &= ~_BV(1); //shift clock down
		delay(10);
	}
	PORTB |= _BV(0); //Store clock up
	delay(10);
	PORTB |= _BV(0); //Store clock down
	delay(10);
	
	cli();
	TCCR1A = 0;
	TCCR1B = 1; // 1/1
	TCCR1C = 0;
	
	for(;;)
	{
		TCNT1 = 0;
		PORTD |= _BV(3);//PNP on H
		PORTD &= ~_BV(5);//OE on L
		while(TCNT1<time);
		PORTD &= ~_BV(3);//PNP disable L
		PORTD |= _BV(5);//OE disable H
		while(TCNT1<total);
	}
	
}

void loop()
{

}
