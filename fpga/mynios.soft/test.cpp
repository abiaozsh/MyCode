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
  uart_write('\r');
  uart_write('\n');

}


int testbne(int a,int b){  if(a==b){    return 1;  }else{    return 2;  }}
int testbeq(int a,int b){  if(a!=b){    return 1;  }else{    return 2;  }}
int testbge(int a,int b){  if(a<b){    return 1;  }else{    return 2;  }}
int testblt(int a,int b){  if(a>=b){    return 1;  }else{    return 2;  }}
int testbltu(unsigned int a, unsigned int b){  if(a>=b){    return 1;  }else{    return 2;  }}
int testbgeu(unsigned int a, unsigned int b){  if(a<b){    return 1;  }else{    return 2;  }}

int testandi(int a){  return a & 0x55AA;}
int testandhi(int a){  return a & 0x55AA0000;}
int testaddi(int a){  return a + 0x55AA;}
int testaddi2(int a){  return a + 0xAA55;}
int testorhi(int a){  return a | 0x55AA0000;}
int testori(int a){  return a | 0x55AA;}
int testxori(int a){  return a ^ 0x55AA;}
int testxorhi(int a){  return a ^ 0x55AA0000;}

//muli reg, reg, ins           @          10 @                      0 @  36 @          100100      0x24
//cmpeqi reg, reg, ins         @          10 @                      0 @  32 @          100000      0x20
//cmplti reg, reg, ins         @          10 @                      0 @  16 @          010000      0x10
//cmpltui reg, reg, ins        @          10 @                      0 @  48 @          110000      0x30
//cmpgei reg, reg, ins         @          10 @                      0 @   8 @          001000      0x08
//cmpnei reg, reg, ins         @          10 @                      0 @  24 @          011000      0x18
//cmpgeui reg, reg, ins        @          10 @                      0 @  40 @          101000      0x28


int main(){
  uart_write('t');
  uart_write('e');
  uart_write('s');
  uart_write('t');
  uart_write('\r');
  uart_write('\n');
  int res;

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
  res = testandhi(0x11111111);printHex(res);
  res = testandhi(0x00000000);printHex(res);
  
//int testaddi(int a){  return a + 0x55AA;}
//int testaddi2(int a){  return a + 0xAA55;}
//int testorhi(int a){  return a | 0x55AA0000;}
//int testori(int a){  return a | 0x55AA;}
//int testxori(int a){  return a ^ 0x55AA;}
//int testxorhi(int a){  return a ^ 0x55AA0000;}

  while(1){
    
  }
}
