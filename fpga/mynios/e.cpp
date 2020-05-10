#include "inc/io.h"
#include "inc/system.h"
#include "inc/system.cpp"
//#include "inc/uart.cpp"
//#include "inc/print.cpp"

void setIrq(int val){
  __asm__ __volatile__("setirq %[input1],%[input1],1"::[input1]"r"(val));
}
void clrIrq(int val){
  __asm__ __volatile__("setirq %[input1],%[input1],0"::[input1]"r"(val));
}

int irq_proc(){
  int irq = IORD(IRQ, 0);
  if(irq&1){
    //timer
    IOWR(MYUART, 1 , 't');
    IOWR(IRQ, 0, 1);
  }
  if(irq&2){
    int tmp = IORD(MYUART, 0);
    IOWR(MYUART, 1 ,tmp);
    IOWR(IRQ, 0, 2);
  }
  
  
  asm("reti");
}

int main(){
  
  IOWR(MYTIMER, 4, 1000000);
  IOWR(MYTIMER, 0, 0);
  setIrq((int)irq_proc);
  
  
  //print("Hello from video demo\r\n");
  
  while(1){};
}
