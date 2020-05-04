//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"

void printBin(int data2){
    uart_write((data2>>11)&1?'1':'0'); // Status Byte
    uart_write((data2>>10)&1?'1':'0'); // Status Byte
    uart_write((data2>>9)&1?'1':'0'); // Status Byte
    uart_write((data2>>8)&1?'1':'0'); // Status Byte
    uart_write((data2>>7)&1?'1':'0'); // Status Byte
    uart_write((data2>>6)&1?'1':'0'); // Status Byte
    uart_write((data2>>5)&1?'1':'0'); // Status Byte
    uart_write((data2>>4)&1?'1':'0'); // Status Byte
    uart_write((data2>>3)&1?'1':'0'); // Status Byte
    uart_write((data2>>2)&1?'1':'0'); // Status Byte
    uart_write((data2>>1)&1?'1':'0'); // Status Byte
    uart_write((data2>>0)&1?'1':'0'); // Status Byte
  
}

int main(){
  printInt(-1);
  print("Hello from Nios II! demo b\r\n");
  while(1){
    //0xeb //0111010110
    //1D6
    //3D6
    
    //ff4
    //c10
    //e00
    //1111 1111 0100
    //1100 0001 0000
    //1110 0000 0000
    
    //#define MYMOUSE      0x02070000
    //#define MYMOUSE_GET  0
    //#define MYMOUSE_SEND 1

    IOWR(MYMOUSE, MYMOUSE_SEND ,0x01EB);
    while(IORD(MYMOUSE, MYMOUSE_SEND));
    int data1=-1;
    int data2=-1;
    int data3=-1;
    int data4=-1;
    int tmp;
    
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data1 = tmp;//(tmp>>3) & 0xFF;
        break;
      }
    }
    
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data2 = tmp;//(tmp>>3) & 0xFF;
        break;
      }
    }
    
    
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data3 = tmp;//(tmp>>3) & 0xFF;
        break;
      }
    }
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data4 = tmp;//(tmp>>3) & 0xFF;
        break;
      }
    }
    
    
    
    print("data1:");
    printBin(data1);
    print("  data2:");
    printBin(data2);
    print("  data3:");
    printBin(data3);
    print("  data4:");
    printBin(data4);
    print("\r\n");
    
    uart_read(-1);

    /*
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
    }*/
  }
}
