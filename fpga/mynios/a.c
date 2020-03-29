//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"

int uart_read(){
  IOWR(MYTIMER, 0 ,0);
  while(1){
    int timer = IORD(MYTIMER, 0);
    if(timer>1000000){
      return 0;
    }
    //int tmp = in32(0x02002000);
    int tmp = IORD(MYUART, 0);
    if(tmp&0x100){
      return tmp;
    }
  }
}

int uart_write(int val){
  //while((in32(0x02002004)) & 0x100);
  //out32(0x02002004,val);
  while((IORD(MYUART, 1)) & 0x100);
  IOWR(MYUART, 1 ,val);
}
int test(){
  return;
}
int main(){
  //asm("hlt 1");
  //asm("hlt 0");
  while(1){
    int val1 = uart_read();
    if(val1==0){
      uart_write('-');
    }else{
      uart_write(val1);
    }
  }
}
