//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"


//int b[1000000];
char* bb = "bbbbaaaaa";

int main(){
  //sdrambuff = (char*)(0);
  print("Hello from Nios II! demo b\r\n");
  while(1){
    print("input:\r\n");
    char buff[5];
    scan(buff,4,-1);
    print(buff);
    printInt(equal(buff,"abcd",1));
    printInt(equal(buff,"abcd",2));
    
    //b[999999] = 1;
    print("\r\n");
    printInt(123);
    print("\r\n");
    printInt(buff[0]);
    print(" ");
    printByte(buff[0]);
    print("\r\n");
    
    printInt(buff[1]);
    print(" ");
    printByte(buff[1]);
    print("\r\n");
    
    print("\r\n");
    print("fdsa");
    print("\r\n");
    print(bb, 5);
    print("\r\n");
    
    if(equal(buff,"test",3)){
      print("input int:");
      int val = scanInt();
      printInt(val);
    }
  }
}
