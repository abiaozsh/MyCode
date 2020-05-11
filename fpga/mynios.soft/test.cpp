#include "inc/io.h"
#include "inc/system.h"
#include "inc/system.cpp"


int uart_write(int val){
  while((IORD(MYUART, 1)) & 0x100);
  IOWR(MYUART, 1 ,val);
}


int printHex(int val){
  char* chardata = "0123456789ABCDEF";
  uart_write(chardata[(val>>28)&0x0F]);
  uart_write(chardata[(val>>24)&0x0F]);
  uart_write(chardata[(val>>20)&0x0F]);
  uart_write(chardata[(val>>16)&0x0F]);
  uart_write(chardata[(val>>12)&0x0F]);
  uart_write(chardata[(val>>8)&0x0F]);
  uart_write(chardata[(val>>4)&0x0F]);
  uart_write(chardata[(val)&0x0F]);
}


int testbne(int a,int b){
  if(a==b){
    return 1;
  }else{
    return 2;
  }
}

int testbeq(int a,int b){
  if(a!=b){
    return 1;
  }else{
    return 2;
  }
}

int testbge(int a,int b){
  if(a<b){
    return 1;
  }else{
    return 2;
  }
}


int testblt(int a,int b){
  if(a>=b){
    return 1;
  }else{
    return 2;
  }
}


int testbltu(unsigned int a, unsigned int b){
  if(a>=b){
    return 1;
  }else{
    return 2;
  }
}

int testbgeu(unsigned int a, unsigned int b){
  if(a<b){
    return 1;
  }else{
    return 2;
  }
}


int testandi(int a){
  return a & 0x55AA;
}
int testandhi(int a){
  return a & 0x55AA0000;
}


int main(){
  uart_write('t');
  uart_write('e');
  uart_write('s');
  uart_write('t');
  int res;
  while(1){
    res = testbne(0x00000000,0x11111111);printHex(res);
    res = testbne(0x11111111,0x11111111);printHex(res);
    
    res = testbeq(0x00000000,0x11111111);printHex(res);
    res = testbeq(0x11111111,0x11111111);printHex(res);
    
    res = testbge(0x00000000,0x11111111);printHex(res);
    res = testbge(0x11111111,0x11111111);printHex(res);
    res = testbge(0x11111111,0x00000000);printHex(res);

    res = testblt(0x00000000,0x11111111);printHex(res);
    res = testblt(0x11111111,0x11111111);printHex(res);
    res = testblt(0x11111111,0x00000000);printHex(res);

    res = testbltu(0x00000000,0x11111111);printHex(res);
    res = testbltu(0x11111111,0x11111111);printHex(res);
    res = testbltu(0x11111111,0x00000000);printHex(res);
    
    res = testbgeu(0x00000000,0x11111111);printHex(res);
    res = testbgeu(0x11111111,0x11111111);printHex(res);
    res = testbgeu(0x11111111,0x00000000);printHex(res);
    
    res = testandi(0x11111111);printHex(res);
    res = testandi(0x00000000);printHex(res);

  }

  while(1){
    
  }
}
