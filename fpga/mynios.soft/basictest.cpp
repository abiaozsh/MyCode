#include "inc/io.h"
#include "inc/system.h"
#include "inc/irq.h"
#include "inc/system.cpp"
#include "inc/uart.cpp"
#include "inc/uartio.cpp"
//#include "inc/keyScreen.cpp"

int main(){
  putc('a');
  putc('b');
  flushCache((void*)(1024*1024));
  putc('c');
  putc('d');
  while(1){
  }


/*
  stdioInit(1024);
  putc('b');
  putc('a');
  putc('s');
  putc('e');
  putc(' ');
  putc('t');
  putc('e');
  putc('s');
  putc('t');
  putc(':');
  while(1){
    int val1 = getc();
    putc(val1);
  }
  */
}
