#include "inc/io.h"
#include "inc/system.h"
#include "inc/irq.h"
#include "inc/system.cpp"
//#include "inc/uart.cpp"
//#include "inc/uartio.cpp"
#include "inc/keyScreen.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/Sd2CardROM.cpp"
#include "inc/FileSystemROM.cpp"


int main(){
  malloc_index = 0;
  stdioInit(1024);
  print("Hello from My DOS 1.0\r\n");

  Sd2Card sdcard;
  SdVolume sdvolume;
  SdFile file;
  
  int res = sdcard.init(0);
  int ok = 0;
  if(res){
    print("found sdcard 0\r\n");
    res = sdvolume.init(&sdcard, 0);
    if(res){
      print("found volume 0\r\n");
      ok = 1;
    }else{
      res = sdvolume.init(&sdcard, 1);
      if(res){
        print("found volume 1\r\n");
        ok = 1;
      }
    }
  }else{
    print("card error 0\r\n");
  }

  if(ok){
    res = file.open(sdvolume.root, "boot.bin", O_READ);
    if(res){
      print("open ok:");printInt(file.fileSize_);print("\r\n");
      
      char* prog_data = (char*)(0);
      
      //int (*prog)(void) = (int(*)(void))prog_data;

      for(int i=0;i<file.fileSize_;i++){
        int val = file.read();
        prog_data[i] = val;
        if(i & 511==0){
          print("loading:");printInt(i);print("\r\n");
        }
      }

      setOff((int)prog_data);
      jmp(0);
      
    }else{
      print("open ng\r\n");
      printInt(file.fileError);print("\r\n");
    }
    
  }else{
      print("can not found volume\r\n");
  }
  
  while(1);
}
