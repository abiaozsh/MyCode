//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"

void dma(){
  int src = 0;
  int des = 0;
  for(int i=0;i<12;i++){
    IOWR(DMA, DMA_SRC_PAGE, 4096 - 256 + src);//at 1Mbyte //4page per line  512byte per page //   int dma_src = trackBar1.Value * 4;//4page per line
    IOWR(DMA, DMA_DES_PAGE, 0 + des);//4page per line  512byte per page //   int dma_src = trackBar1.Value * 4;//4page per line
    IOWR(DMA, DMA_PAGE_LEN, 256);//int page_len = 256;   256page per time       total 12times
    IOWR(DMA, DMA_REQ, 1);
    src+=256;
    des+=256;
  }

}

int main(){

  print("Hello from video demo\r\n");
  
  while(1){
    
    char str[10];
    int res;
    
    scan(str,-1,-1);
    if(equal(str,"v1",-1)){
      IOWR(VGA, VGA_MODE, 0);
    }
    
    if(equal(str,"v2",-1)){
      IOWR(VGA, VGA_MODE, 2);
    }
    
    if(equal(str,"p",-1)){
      int pos = scanInt();
      IOWR(VGA, VGA_BASE, pos);
    }

    
    if(equal(str,"clr",-1)){
      for(int i=0;i<0x80000;i++){
        ((int*)(0x200000))[i] = 0;//at 1Mbyte
      }
      dma();
    }
    
    if(equal(str,"test",-1)){
      int top = scanInt();
      for(int i=0;i<1024;i++){
        for(int j=0;j<768;j++){
          int val1;
          if((i>0 && i<100 && j>0 && j<top) || (i==j)){
            val1 = 0xFFFF;
          }else{
            val1 = 0x0000;
          }
          ((short*)(0x200000))[i+(j<<10)] = val1;//at 1Mbyte
        }
      }
      
      print("donea");
      dma();
      print("doneb");
    }

  }
}
