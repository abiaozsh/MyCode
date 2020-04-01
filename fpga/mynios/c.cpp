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



char* SDdata;



int main()
{
  SDdata = (char*)(0);//at sdram [512]
  
	char buff[4];

	print("Hello from Nios II!\r\n");

	while(1){
    char buff[2];
    buff[0] = (char)uart_read(-1);
    int s;

    
		if(buff[0]=='i'){
			print("init start!\r\n");
			int result = MMCCard_cardinit();
      if(result){
        print("init success!\r\n");
      }else{
        print("init fail!\r\n");
      }
		}

    if(buff[0]=='r'){
      int block;
      
      buff[0] = (char)uart_read(-1);
      if(buff[0]=='0'){
        block = 0;
      }else{
        block = 1;
      }
      
      
      print("[");
      if (Sd2Card_readData(block, SDdata)) {
      int i;
        for(i=0;i<512;i++){
          if((i&15)==0 && i!=0){
            print("\n");
          }
          char data = SDdata[i];
          printByte(data);
          //uart_write(data);
        }
        print("]");
      }
      else {
        print("err:");
        printInt(errCode);
      }

    }


	}
  return 0;
}
