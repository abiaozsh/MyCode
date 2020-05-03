//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"

int printByte(int val){
  char* chardata = "0123456789ABCDEF";
  uart_write(chardata[(val>>4)&0x0F]);
  uart_write(chardata[(val)&0x0F]);
}

int main(){
  uart_write('t');
  uart_write('e');
  uart_write('s');
  uart_write('t');
  uart_write('A');
  while(1){
    int val1 = uart_read(1000000);
    if(val1=='a'){
      uart_write('?');
      int val2 = uart_read()-'0';
      int val3 = uart_read()-'0';
      uart_write(' ');
      printByte(val2);
      uart_write(' ');
      printByte(val3);
      uart_write('\r');
      uart_write('\n');
      
      int val4 = val2/val3;
      int val5 = val2*val3;
      uart_write(' ');
      printByte(val4);
      uart_write(' ');
      printByte(val5);
      uart_write('\r');
      uart_write('\n');

    }else{
      uart_write('*');
    }
  }
}
