//中断向量
//浮点数

//#define DEBUG

#include "inc/io.h"
#include "inc/system.h"
#include "inc/noirq.h"
#include "inc/system.cpp"
#include "inc/uart.cpp"
#include "inc/uartio.cpp"
#include "inc/print.cpp"

int main()
{

  while(1){
    print("v1?\r\n");
    int aa = scanInt();
    print("v2?\r\n");
    int bb = scanInt();
    asm("hlt 1");
    asm("hlt 0");
    int cc = aa<<3;
    printInt(cc);
  }

  return 0;
  
}
