#include "inc/io.h"
#include "inc/system.h"
#include "inc/system.cpp"
#include "inc/uart.cpp"
#include "inc/uartio.cpp"
//#include "inc/keyScreen.cpp"
#include "inc/print.cpp"

void SPI_CHIP_SELECT_HIGH(){
  IOWR(SPIROM, SPIROM_CS, 1);
}
void SPI_CHIP_SELECT_LOW(){
  IOWR(SPIROM, SPIROM_CS, 0);
}

void SPI_WRITE_POLPHA(int val){
  IOWR(SPIROM, SPIROM_POLPHA, val);
}

//------------------------------------------------------------------------------
//volatile int spi_debug0;
int spiRec() {
  IORD(SPIROM, SPIROM_READ);
  IOWR(SPIROM, SPIROM_READ, 1);
  //spi_debug0 = 0;
  while(1){
    //spi_debug0++;
    int tmp = IORD(SPIROM, SPIROM_READ);
    if(tmp & 0x100){
      return tmp & 0xFF;
    }
  }
}

//volatile int spi_debug1;
//------------------------------------------------------------------------------
void spiSend(int data) {
  IORD(SPIROM, SPIROM_WRITE);
  IOWR(SPIROM, SPIROM_WRITE, data);
  //spi_debug1 = 0;
  while(1){
    //spi_debug1++;
    int tmp = IORD(SPIROM, SPIROM_WRITE);
    if(tmp & 0x100){
      return;
    }
  }
}

int main()
{
  malloc_index = 0;
  
  stdioInit(1024);

	char str[20];

	print("Hello from Nios II!\r\n");
  
  SPI_WRITE_POLPHA(15);
  
  SPI_CHIP_SELECT_LOW();
  
  spiSend(0x9F);
  printInt(spiRec());print("\r\n");
  printInt(spiRec());print("\r\n");
  printInt(spiRec());print("\r\n");
  printInt(spiRec());print("\r\n");
  
 
  SPI_CHIP_SELECT_HIGH();
	while(1){

	}
  return 0;
}
