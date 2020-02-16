#include <stdio.h>
#include "system.h"
#include "sys/alt_flash.h"
#include "altera_avalon_pio_regs.h"

int main() {
	int data = 0;

    flash_region* regions_info;
    alt_flash_fd* fd;
    int number_of_regions;
    int ret_code;

    while (1) {

		int val = IORD(PIO_1_BASE, 0);

		if(val & 1){
			data++;
		}

		IOWR(PIO_0_BASE, 0, data);

		char s[50];
		scanf("%s", s);
		printf("%s", s);


		fd = alt_flash_open_dev(EPCS_NAME);
	    if(!fd){
	        printf("Can't open flash device\n");
	    }else{
	    	printf("ok\n");
	    	ret_code = alt_get_flash_info(fd, &regions_info, &number_of_regions);
	    	printf("%d\n",ret_code);
	    	printf("%d\n",number_of_regions);
	    	if(ret_code==0){
				printf("Flash Region:\n");
				printf("region_number = %d\n"	   ,number_of_regions);
				printf("region_offset = %d\n"      ,regions_info->offset);
				printf("region_size   = %d Bytes\n",regions_info->region_size);
				printf("block_number  = %d\n"	   ,regions_info->number_of_blocks);
				printf("block_size    = %d Bytes\n",regions_info->block_size);

	    	}
	    }

	}

	return 0;
}

