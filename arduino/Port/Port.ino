//http://arduino.cc/en/Hacking/PinMapping2560
//http://arduino.cc/en/Hacking/PinMapping32u4

/*
28	(ADC11/PCINT4) PB4	Digital pin 8	B	4	8
29	(PCINT5/OC1A/#OC4B/ADC12) PB5	Digital Pin 9 (PWM)	B	5	9
30	(PCINT6/OC1B/OC4B/ADC13) PB6	Digital Pin 10 (PWM)	B	6	10
12	(PCINT7/OCA0/OC1C/#RTS) PB7	Digital pin 11 (PWM)	B	7	11
31	(OC3A/#0C4A) PC6	Digital Pin 5 (PWM)	C	6	5
32	(ICP3/CLK0/)C4A) PC7	Digital Pin 13 (PWM)	C	7	13
18	(OC0B/SCL/INT0) PD0	Digital pin 3 (SCL)(PWM)	D	0	3
19	(SDA/INT1) PD1	Digital pin 2 (SDA)	D	1	2
20	(RX D1/AIN1/INT2) PD2	Digital pin 0 (RX)	D	2	0
21	(TXD1/INT3) PD3	Digital pin 1 (TX)	D	3	1
25	(ICP1/ADC8) PD4	Digital pin 4	D	4	4
26	(T1/#OC4D/ADC9) PD6	Digital pin 12	D	6	12
27	(T0/OC4D/ADC10) PD7	Digital Pin 6 (PWM)	D	7	6
1	PE6 (INT.6/AIN0)	Digital pin 7	E	6	7
36	(ADC7/TDI) PF7	Analog In 0			A
37	(ADC6/TDO) PF6	Analog In 1			A
38	(ADC5/TMS) PF5	Analog In 2			A
39	(ADC4/TCK) PF4	Analog In 3			A
40	(ADC1) PF1	Analog In 4			A
41	(ADC0) PF0	Analog In 5			A
2	UVcc	+5V			X
3	D-	RD-			X
4	D+	RD+			X
5	UGnd	UGND			X
6	UCap	UCAP			X
7	VUSB	VBus			X
8	(SS/PCINT0) PB0	RXLED			X
9	(PCINT1/SCLK) PB1	SCK			X
10	(PDI/PCINT2/MOSI) PB2	MOSI			X
11	(PDO/PCINT3/MISO) PB3	MISO			X
13	RESET	RESET			X
14	Vcc	+5V			X
15	GND	GND			X
16	XTAL2	XTAL2			X
17	XTAL1	XTAL1			X
22	(XCK1/#CTS) PD5	TXLED			X
23	GND1	GND			X
24	AVCC	AVCC			X
33	(#HWB) PE2	HWB			X
34	Vcc1	+5V			X
35	GND2	GND			X
42	AREF	AEF			X
43	GND3	GND			X
44	AVCC1	AVCC			X



1	PG5 ( OC0B )	Digital pin 4 (PWM)	G	5	4
2	PE0 ( RXD0/PCINT8 )	Digital pin 0 (RX0)	E	0	0
3	PE1 ( TXD0 )	Digital pin 1 (TX0)	E	1	1
4	PE2 ( XCK0/AIN0 )				x
5	PE3 ( OC3A/AIN1 )	Digital pin 5 (PWM)	E	3	5
6	PE4 ( OC3B/INT4 )	Digital pin 2 (PWM)	E	4	2
7	PE5 ( OC3C/INT5 )	Digital pin 3 (PWM)	E	5	3
8	PE6 ( T3/INT6 )				x
9	PE7 ( CLKO/ICP3/INT7 )				x
10	VCC	VCC			x
11	GND	GND			x
12	PH0 ( RXD2 )	Digital pin 17 (RX2)			17
13	PH1 ( TXD2 )	Digital pin 16 (TX2)			16
14	PH2 ( XCK2 )				x
15	PH3 ( OC4A )	Digital pin 6 (PWM)			6
16	PH4 ( OC4B )	Digital pin 7 (PWM)			7
17	PH5 ( OC4C )	Digital pin 8 (PWM)			8
18	PH6 ( OC2B )	Digital pin 9 (PWM)			9
19	PB0 ( SS/PCINT0 )	Digital pin 53 (SS)			53
20	PB1 ( SCK/PCINT1 )	Digital pin 52 (SCK)			54
21	PB2 ( MOSI/PCINT2 )	Digital pin 51 (MOSI)			51
22	PB3 ( MISO/PCINT3 )	Digital pin 50 (MISO)			50
23	PB4 ( OC2A/PCINT4 )	Digital pin 10 (PWM)			10
24	PB5 ( OC1A/PCINT5 )	Digital pin 11 (PWM)			11
25	PB6 ( OC1B/PCINT6 )	Digital pin 12 (PWM)			12
26	PB7 ( OC0A/OC1C/PCINT7 )	Digital pin 13 (PWM)			13
27	PH7 ( T4 )				x
28	PG3 ( TOSC2 )				x
29	PG4 ( TOSC1 )				x
30	RESET	RESET			x
31	VCC	VCC			x
32	GND	GND			x
33	XTAL2	XTAL2			x
34	XTAL1	XTAL1			x
35	PL0 ( ICP4 )	Digital pin 49			49
36	PL1 ( ICP5 )	Digital pin 48			48
37	PL2 ( T5 )	Digital pin 47			47
38	PL3 ( OC5A )	Digital pin 46 (PWM)			46
39	PL4 ( OC5B )	Digital pin 45 (PWM)			47
40	PL5 ( OC5C )	Digital pin 44 (PWM)			44
41	PL6	Digital pin 43			43
42	PL7	Digital pin 42			42
43	PD0 ( SCL/INT0 )	Digital pin 21 (SCL)			21
44	PD1 ( SDA/INT1 )	Digital pin 20 (SDA)			20
45	PD2 ( RXDI/INT2 )	Digital pin 19 (RX1)			19
46	PD3 ( TXD1/INT3 )	Digital pin 18 (TX1)			18
47	PD4 ( ICP1 )				x
48	PD5 ( XCK1 )				x
49	PD6 ( T1 )				x
50	PD7 ( T0 )	Digital pin 38			38
51	PG0 ( WR )	Digital pin 41			41
52	PG1 ( RD )	Digital pin 40			40
53	PC0 ( A8 )	Digital pin 37			37
54	PC1 ( A9 )	Digital pin 36			36
55	PC2 ( A10 )	Digital pin 35			35
56	PC3 ( A11 )	Digital pin 34			34
57	PC4 ( A12 )	Digital pin 33			33
58	PC5 ( A13 )	Digital pin 32			32
59	PC6 ( A14 )	Digital pin 31			31
60	PC7 ( A15 )	Digital pin 30			30
61	VCC	VCC			x
62	GND	GND			x
63	PJ0 ( RXD3/PCINT9 )	Digital pin 15 (RX3)			15
64	PJ1 ( TXD3/PCINT10 )	Digital pin 14 (TX3)			14
65	PJ2 ( XCK3/PCINT11 )				x
66	PJ3 ( PCINT12 )				x
67	PJ4 ( PCINT13 )				x
68	PJ5 ( PCINT14 )				x
69	PJ6 ( PCINT 15 )				x
70	PG2 ( ALE )	Digital pin 39			39
71	PA7 ( AD7 )	Digital pin 29			29
72	PA6 ( AD6 )	Digital pin 28			28
73	PA5 ( AD5 )	Digital pin 27			27
74	PA4 ( AD4 )	Digital pin 26			26
75	PA3 ( AD3 )	Digital pin 25			25
76	PA2 ( AD2 )	Digital pin 24			24
77	PA1 ( AD1 )	Digital pin 23			23
78	PA0 ( AD0 )	Digital pin 22			22
79	PJ7				x
80	VCC	VCC			x
81	GND	GND			x
82	PK7 ( ADC15/PCINT23 )	Analog pin 15			a
83	PK6 ( ADC14/PCINT22 )	Analog pin 14			a
84	PK5 ( ADC13/PCINT21 )	Analog pin 13			a
85	PK4 ( ADC12/PCINT20 )	Analog pin 12			a
86	PK3 ( ADC11/PCINT19 )	Analog pin 11			a
87	PK2 ( ADC10/PCINT18 )	Analog pin 10			a
88	PK1 ( ADC9/PCINT17 )	Analog pin 9			a
89	PK0 ( ADC8/PCINT16 )	Analog pin 8			a
90	PF7 ( ADC7 )	Analog pin 7			a
91	PF6 ( ADC6 )	Analog pin 6			a
92	PF5 ( ADC5/TMS )	Analog pin 5			a
93	PF4 ( ADC4/TMK )	Analog pin 4			a
94	PF3 ( ADC3 )	Analog pin 3			a
95	PF2 ( ADC2 )	Analog pin 2			a
96	PF1 ( ADC1 )	Analog pin 1			a
97	PF0 ( ADC0 )	Analog pin 0			a
98	AREF	Analog Reference			x
99	GND	GND			x
100	AVCC	VCC			x



*/


//#declare digitalPinMap pmap2560 

struct pin_map_t {
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  volatile uint8_t* port;
  uint8_t bit;
};

static const pin_map_t digitalPinMap[] = {//pmap2560
  {&DDRE, &PINE, &PORTE, 0},  // E0  0
  {&DDRE, &PINE, &PORTE, 1},  // E1  1
  {&DDRE, &PINE, &PORTE, 4},  // E4  2
  {&DDRE, &PINE, &PORTE, 5},  // E5  3
  {&DDRG, &PING, &PORTG, 5},  // G5  4
  {&DDRE, &PINE, &PORTE, 3},  // E3  5
  {&DDRH, &PINH, &PORTH, 3},  // H3  6
  {&DDRH, &PINH, &PORTH, 4},  // H4  7
  {&DDRH, &PINH, &PORTH, 5},  // H5  8
  {&DDRH, &PINH, &PORTH, 6},  // H6  9
  {&DDRB, &PINB, &PORTB, 4},  // B4 10
  {&DDRB, &PINB, &PORTB, 5},  // B5 11
  {&DDRB, &PINB, &PORTB, 6},  // B6 12
  {&DDRB, &PINB, &PORTB, 7},  // B7 13
  {&DDRJ, &PINJ, &PORTJ, 1},  // J1 14
  {&DDRJ, &PINJ, &PORTJ, 0},  // J0 15
  {&DDRH, &PINH, &PORTH, 1},  // H1 16
  {&DDRH, &PINH, &PORTH, 0},  // H0 17
  {&DDRD, &PIND, &PORTD, 3},  // D3 18
  {&DDRD, &PIND, &PORTD, 2},  // D2 19
  {&DDRD, &PIND, &PORTD, 1},  // D1 20
  {&DDRD, &PIND, &PORTD, 0},  // D0 21
  {&DDRA, &PINA, &PORTA, 0},  // A0 22
  {&DDRA, &PINA, &PORTA, 1},  // A1 23
  {&DDRA, &PINA, &PORTA, 2},  // A2 24
  {&DDRA, &PINA, &PORTA, 3},  // A3 25
  {&DDRA, &PINA, &PORTA, 4},  // A4 26
  {&DDRA, &PINA, &PORTA, 5},  // A5 27
  {&DDRA, &PINA, &PORTA, 6},  // A6 28
  {&DDRA, &PINA, &PORTA, 7},  // A7 29
  {&DDRC, &PINC, &PORTC, 7},  // C7 30
  {&DDRC, &PINC, &PORTC, 6},  // C6 31
  {&DDRC, &PINC, &PORTC, 5},  // C5 32
  {&DDRC, &PINC, &PORTC, 4},  // C4 33
  {&DDRC, &PINC, &PORTC, 3},  // C3 34
  {&DDRC, &PINC, &PORTC, 2},  // C2 35
  {&DDRC, &PINC, &PORTC, 1},  // C1 36
  {&DDRC, &PINC, &PORTC, 0},  // C0 37
  {&DDRD, &PIND, &PORTD, 7},  // D7 38
  {&DDRG, &PING, &PORTG, 2},  // G2 39
  {&DDRG, &PING, &PORTG, 1},  // G1 40
  {&DDRG, &PING, &PORTG, 0},  // G0 41
  {&DDRL, &PINL, &PORTL, 7},  // L7 42
  {&DDRL, &PINL, &PORTL, 6},  // L6 43
  {&DDRL, &PINL, &PORTL, 5},  // L5 44
  {&DDRL, &PINL, &PORTL, 4},  // L4 45
  {&DDRL, &PINL, &PORTL, 3},  // L3 46
  {&DDRL, &PINL, &PORTL, 2},  // L2 47
  {&DDRL, &PINL, &PORTL, 1},  // L1 48
  {&DDRL, &PINL, &PORTL, 0},  // L0 49
  {&DDRB, &PINB, &PORTB, 3},  // B3 50
  {&DDRB, &PINB, &PORTB, 2},  // B2 51
  {&DDRB, &PINB, &PORTB, 1},  // B1 52
  {&DDRB, &PINB, &PORTB, 0},  // B0 53
  {&DDRF, &PINF, &PORTF, 0},  // F0 54
  {&DDRF, &PINF, &PORTF, 1},  // F1 55
  {&DDRF, &PINF, &PORTF, 2},  // F2 56
  {&DDRF, &PINF, &PORTF, 3},  // F3 57
  {&DDRF, &PINF, &PORTF, 4},  // F4 58
  {&DDRF, &PINF, &PORTF, 5},  // F5 59
  {&DDRF, &PINF, &PORTF, 6},  // F6 60
  {&DDRF, &PINF, &PORTF, 7},  // F7 61
  {&DDRK, &PINK, &PORTK, 0},  // K0 62
  {&DDRK, &PINK, &PORTK, 1},  // K1 63
  {&DDRK, &PINK, &PORTK, 2},  // K2 64
  {&DDRK, &PINK, &PORTK, 3},  // K3 65
  {&DDRK, &PINK, &PORTK, 4},  // K4 66
  {&DDRK, &PINK, &PORTK, 5},  // K5 67
  {&DDRK, &PINK, &PORTK, 6},  // K6 68
  {&DDRK, &PINK, &PORTK, 7}   // K7 69
};		
static const pin_map_t pmappro[] = {
  {&DDRD, &PIND, &PORTD, 2},  // D2  0
  {&DDRD, &PIND, &PORTD, 3},  // D3  1
  {&DDRD, &PIND, &PORTD, 1},  // D1  2
  {&DDRD, &PIND, &PORTD, 0},  // D0  3
  {&DDRD, &PIND, &PORTD, 4},  // D4  4
  {&DDRC, &PINC, &PORTC, 6},  // C6  5
  {&DDRD, &PIND, &PORTD, 7},  // D7  6
  {&DDRE, &PINE, &PORTE, 6},  // E6  7
  {&DDRB, &PINB, &PORTB, 4},  // B4  8
  {&DDRB, &PINB, &PORTB, 5},  // B5  9
  {&DDRB, &PINB, &PORTB, 6},  // B6 10
  {&DDRB, &PINB, &PORTB, 7},  // B7 11
  {&DDRD, &PIND, &PORTD, 6},  // D6 12
  {&DDRC, &PINC, &PORTC, 7},  // C7 13
  {&DDRB, &PINB, &PORTB, 3},  // B3 14
  {&DDRB, &PINB, &PORTB, 1},  // B1 15
  {&DDRB, &PINB, &PORTB, 2},  // B2 16
  {&DDRB, &PINB, &PORTB, 0},  // B0 17
  {&DDRF, &PINF, &PORTF, 7},  // F7 18
  {&DDRF, &PINF, &PORTF, 6},  // F6 19
  {&DDRF, &PINF, &PORTF, 5},  // F5 20
  {&DDRF, &PINF, &PORTF, 4},  // F4 21
  {&DDRF, &PINF, &PORTF, 1},  // F1 22
  {&DDRF, &PINF, &PORTF, 0},  // F0 23
};		
	
/*
DDRD = B11111110;  // sets Arduino pins 1 to 7 as outputs, pin 0 as input
DDRD = DDRD | B11111100;  // this is safer as it sets pins 2 to 7 as outputs
	                  // without changing the value of pins 0 & 1, which are RX & TX 
PORTD = B10101000; // sets digital pins 7,5,3 HIGH
*/

uint8_t _getmode(uint8_t pin) {
    return (*digitalPinMap[pin].ddr >> digitalPinMap[pin].bit) & 1;
}

void _setmode(uint8_t pin, uint8_t mode) {
    if (mode) {
      *digitalPinMap[pin].ddr |= 1 << digitalPinMap[pin].bit;
    } else {
      *digitalPinMap[pin].ddr &= ~(1 << digitalPinMap[pin].bit);
    }
}

uint8_t _readpin(uint8_t pin) {
    return (*digitalPinMap[pin].pin >> digitalPinMap[pin].bit) & 1;
}

void _setpin(uint8_t pin, uint8_t value) {
    if (value) {
      *digitalPinMap[pin].port |= 1 << digitalPinMap[pin].bit;
    } else {
      *digitalPinMap[pin].port &= ~(1 << digitalPinMap[pin].bit);
    }
}
void setup()
{
  while(1)
  {
    _setpin(1,1);
  }
  
}

void loop()
{

}
