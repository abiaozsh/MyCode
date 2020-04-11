//函数指针
//变量指针
//中断向量
//浮点数
#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/Sd2Card.cpp"
#include "inc/FileSystem.cpp"


int main(){
  Sd2Card* sdcard = (Sd2Card*)(0x800000);//at8M
  SdVolume* sdvolume = (SdVolume*)(0x810000);//at8M~
  SdFile* root = (SdFile*)(0x820000);//at8M~
  SdFile* file = (SdFile*)(0x830000);//at8M~
  cid_t* cid = (cid_t*)(0x840000);//at8M~
  csd_t* csd = (csd_t*)(0x850000);//at8M~
  
  print("Hello from My DOS\r\n");
  
  while(1){
    
    char str[10];
    int res;
    
    scan(str,-1,-1);
    if(equal(str,"i",-1)){
      print("which sd?\r\n");
      int cs = scanInt();
      res = sdcard->init(cs);
      if(res){
        print("sd ok\r\n");
        sdcard->printCID(cid);
        sdcard->printCSD(csd);
        print("size:");printInt(sdcard->cardSize());print("\r\n");
        
        sdcard->printType();
        print("which partition?\r\n");
        int part = scanInt();
        res = sdvolume->init(sdcard, part);
        if(res){
          print("volume ok\r\n");
          res = root->openRoot(sdvolume);
          if(res){
            print("root ok\r\n");
          }else{
            print("root ng\r\n");
          }
        }else{
          print("volume ng\r\n");
          printInt(sdvolume->error);
        }
      }else{
        print("sd ng\r\n");
        printInt(sdcard->errorCode());
      }
    }
    
    if(equal(str,"dir",-1)){
      root->dirList();
    }
    
    if(equal(str,"mkdir",-1)){
      print("dir name?\r\n");
      char filename[12];
      scan(filename,-1,-1);
      res = file->makeDir(root, filename);
      if(res){
        print("mkdir ok\r\n");
      }else{
        print("mkdir ng\r\n");
      }

    }
    
    if(equal(str,"o",-1)){
      print("which file?\r\n");
      char filename[12];
      scan(filename,-1,-1);
      res = file->open(root, filename, O_READ);
      if(res){
        print("open ok\r\n");
        printInt(file->fileSize_);print("\r\n");
        for(int i=0;i<file->fileSize_;i++){
          int val = file->read();
          uart_write(val);
        }
      }else{
        print("open ng\r\n");
        printInt(file->fileError);print("\r\n");
      }
    }

  }
}
