//#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"

volatile int aa = 0;
int main() {
	int data = 0;
	int oldval = 0;
	while (1) {
		int val = IORD(PIO_1_BASE, 0);

		aa = IORD(0x0030, 0);

		if(val != oldval){
			data++;
			IOWR(0x0030, 0, data);
			IOWR(PIO_0_BASE, 0, data);
		}
		oldval = val;


	}
	return 0;
}

