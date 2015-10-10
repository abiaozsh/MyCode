#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>

#define IRPIN (PINA & _BV(1))
#include "D:\MyCode\arduino\_Common\ir.h"

#define CUR_TIMING TIMING__8M_TCCR0B_1_115200
#define TCCR0B_Value 1
#define DDR_Send DDRB
#define PORT_Send PORTB
#define BIT_Send _BV(0)
#define DDR_Recv DDRB
#define PIN_Recv PINB
#define BIT_Recv _BV(1)
#include "D:\MyCode\arduino\_Common\serial.h"


void ClockInit();
void loop();

int main(void) {
	ClockInit();
	SerialInit();
	SerialSend('f');
	loop();
}

void ClockInit() {
	CLKPR = 128;//The CLKPCE bit must be written to logic one to enable change of the CLKPS bits. The CLKPCE bit is only updated when the other bits in CLKPR are simultaniosly written to zero.
	//CLKPR = 3;//1/8
	CLKPR = 0;//1/1 //8MHz
}

uint8_t buff[8];
void loop() {
	for(;;)
	{
	//RAWTimeline();
	uint8_t v = GetIR();
	SendInt(v);
	SerialSend('\r');
	SerialSend('\n');
	}
}


