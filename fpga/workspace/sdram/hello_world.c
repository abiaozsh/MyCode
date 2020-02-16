#include <stdio.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"

int main() {
	int data = 0;
	while (1) {
		int val = IORD(PIO_1_BASE, 0);

		if(val & 1){
			data++;
		}

		IOWR(PIO_0_BASE, 0, data);

		char s[50];
		scanf("%s", s);
		printf("%s", s);

	}

	return 0;
}

