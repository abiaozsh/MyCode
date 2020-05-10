//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"


int main(){
  printInt(-1);
  print("Hello from Nios II! demo b\r\n");
  
  //base + clr
  IOWR(VGA, VGA_BASE, 1024);//1024=2Mbyte
  for(int i=0;i<0x80000;i++){
    ((int*)(0x200000))[i] = 0;//at 2Mbyte
  }

  int x = 0;
  int y = 0;
  
  while(1){

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
        data1 = (tmp>>2) & 0xFF;
        break;
      }
    }
    
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data2 = (tmp>>2) & 0xFF;
        break;
      }
    }
    
    
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data3 = (tmp>>2) & 0xFF;
        break;
      }
    }
    if(data2&(1<<4)){
      data3|=0xFFFFFF00;
    }
    
    IOWR(MYMOUSE, MYMOUSE_GET ,1);
    while(1){
      tmp = IORD(MYMOUSE, MYMOUSE_GET);
      if(tmp&0x800){
        data4 = (tmp>>2) & 0xFF;
        break;
      }
    }
    if(data2&(1<<5)){
      data4|=0xFFFFFF00;
    }

    x+=data3;
    if(x<0){
      x=0;
    }
    if(x>1023){
      x=1023;
    }
    y-=data4;
    if(y<0){
      y=0;
    }
    if(y>767){
      y=767;
    }
    
    //if(data2&0x01){
    //  print("L");
    //}else{
    //  print(" ");
    //}
    //if(data2&0x02){
    //  print("R");
    //}else{
    //  print(" ");
    //}
    //
    //
    //print(" x:");
    //printInt(x);
    //print(" y:");
    //printInt(y);
    //print("\r\n");

    short* p = &(((short*)(0x200000))[x+y*1024]);
    *p = 0xFFFF;//at 2Mbyte
    flushCache(p);

      
    
    if(data2&0x01){
      for(int i=0;i<0x80000;i++){
        ((int*)(0x200000))[i] = 0;//at 2Mbyte
      }
    }

    IOWR(MYTIMER, 0 ,0);
    while(1){
      int timer = IORD(MYTIMER, 0);
      if(timer>50000){
        break;
      }
    }


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
