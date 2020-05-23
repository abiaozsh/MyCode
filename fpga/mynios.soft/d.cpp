#include "inc/io.h"
#include "inc/system.h"
#include "inc/system.cpp"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/Sd2Card.cpp"


uint8_t cardCommand(uint8_t cmd, uint32_t arg) {
  // send command
  spiSend(cmd | 0x40);

  // send argument
  for (int8_t s = 24; s >= 0; s -= 8) spiSend(arg >> s);

  // send CRC
  uint8_t crc = 0XFF;
  if (cmd == CMD0) crc = 0X95;  // correct crc for CMD0 with arg 0
  if (cmd == CMD8) crc = 0X87;  // correct crc for CMD8 with arg 0X1AA
  spiSend(crc);

  // wait for response
  //for (uint8_t i = 0; ((status_ = spiRec()) & 0X80) && i != 0XFF; i++)
    ;
  return 0;
}

int main()
{
              
//            TODO dos d 还需要测试，sd卡0 报错 41  断电后恢复
//            done
//r
//res:F
//r
//res:1

            
  malloc_index = 0;

  Sd2Card* sdcard = (Sd2Card*)malloc(sizeof(Sd2Card));//at8M
  
	char str[20];

	print("Hello from Nios II!\r\n");
  
	while(1){
    scan(str,-1,-1);
    print(str);print("\r\n");
    
    
    if(equal(str,"i",-1)){
      print("which sd?\r\n");
      int cs = scanInt();
      printInt(cs);
      int res = sdcard->init(cs);
      if(res){
        print("sd ok\r\n");

      }else{
        print("sd ng\r\n");
        print("sdcard->initError");printInt(sdcard->initError);print("\r\n");
      }
    }
    
    if(equal(str,"cs",-1)){
      print("which sd?\r\n");
      for (uint8_t i = 0; i < 10; i++) spiSend(0XFF);
      int cs = scanInt();
      printInt(cs);
      sdcard->chipSelectPin_ = cs;
      sdcard->chipSelectLow();
      print("done\r\n");
    }

    if(equal(str,"c",-1)){
      cardCommand(CMD0, 0);
      print("done\r\n");
    }
    
    if(equal(str,"clk",-1)){
      IOWR(SOFTSPI, SOFTSPI_SCK, 1);
      print("ok\r\n");
      IOWR(SOFTSPI, SOFTSPI_SCK, 0);
      print("ok\r\n");
    }

    if(equal(str,"r",-1)){
      int res = spiRec();
      print("res:");printByte(res);
      //print("spi_debug0:");printInt(spi_debug0);
      print("\r\n");
    }
    

	}
  return 0;
}
