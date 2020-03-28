//函数指针
//变量指针
//中断向量
//浮点数
//int out32(int addr,int val){
//  asm("out32 r4, r5");//addr val
//}
//int in32(int addr){
//  asm("in32 r4, r2");//addr val
//}
//#include <stdio.h>
#include "inc/io.h"

int uart_read(){
  while(1){
    //int tmp = in32(0x02002000);
    int tmp = IORD(0x02002000, 0);
    if(tmp&0x100){
      return tmp;
    }
  }
}

int uart_write(int val){
  //while((in32(0x02002004)) & 0x100);
  //out32(0x02002004,val);
  while((IORD(0x02002004, 0)) & 0x100);
  IOWR(0x02002004, 0 ,val);
}
int test(){
  return;
}
int main(){
  //asm("hlt 1");
  //asm("hlt 0");
  test();
  while(1){
    int val1 = uart_read();
    uart_write(val1);
  }
}
