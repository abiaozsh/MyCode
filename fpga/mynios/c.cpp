//函数指针
//变量指针
//中断向量
//浮点数

//#define DEBUG

#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/sd.cpp"



int main()
{
  
  SDcard* sdcard;

  sdcard = (SDcard*)(0);//at sdram [512]
  sdcard->chip_select = 0;
  
	char sbuff[10];

	print("Hello from Nios II!\r\n");

	while(1){
    scan(sbuff,-1,-1);

		if(equal(sbuff,"i",-1)){
      print("which sd?:\r\n");
      
      sdcard->chip_select = scanInt();
      
			print("init start!\r\n");
			int result = MMCCard_cardinit(sdcard);
      if(result){
        print("init success!\r\n");
      }else{
        print("init fail!\r\n");
      }
		}

    if(equal(sbuff,"r",-1)){
      int block;
      print("block?:\r\n");
      block = scanInt();
      printInt(block);

      print("[");
      if (Sd2Card_readData(sdcard, block)) {
      int i;
        for(i=0;i<512;i++){
          if((i&15)==0 && i!=0){
            print("\n");
          }
          char data = sdcard->buff.data[i];
          printByte(data);
          //uart_write(data);
        }
        print("]");
      }
      else {
        print("err:");
      }

    }


	}
  return 0;
}
