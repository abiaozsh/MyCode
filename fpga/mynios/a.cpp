//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"

int main(){
  uart_write('t');
  uart_write('e');
  uart_write('s');
  uart_write('t');
  uart_write('A');
  while(1){
    int val1 = uart_read(1000000);
    if(val1=='a'){
      int val2 = uart_read();
      uart_write('[');
      uart_write(val2);
      uart_write(']');
    }else{
      uart_write('*');
    }
    if(val1==-1){
      uart_write('-');
    }else{
      uart_write(val1);
    }
  }
}
