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


void* __eof__();
int malloc_index = 0;
void* malloc(int size){
  size = (size & (~0x03))+4;
  int idx = (int)__eof__();
  idx += malloc_index;
  malloc_index += size;
  return (void*)idx;
}

void mfree(int size){
  malloc_index -= size;
}


void initDisk(Sd2Card** sdcard,int cardidx, SdVolume** sdvolumes, int* totalVolume){
  int res;
  res = sdcard[cardidx]->init(cardidx);
  if(res){
    print("found card ");printInt(cardidx);print("\r\n");
    SdVolume* tempVol = (SdVolume*)malloc(sizeof(SdVolume));
    
    res = tempVol->init(sdcard[cardidx], 0);
    if(res){
      print("found volume 0\r\n");
      sdvolumes[*totalVolume] = tempVol;
      *totalVolume = *totalVolume + 1;
      return;
    }else{
      res = tempVol->init(sdcard[cardidx], 1);
      if(res){
        print("found volume 1\r\n");
        sdvolumes[*totalVolume] = tempVol;
        *totalVolume = *totalVolume + 1;
        return;
      }
    }
    //mfree(sizeof(SdVolume));
  }else{
    print("card error ");printInt(cardidx);print("\r\n");
  }
}

int main(){

  Sd2Card* sdcard = (Sd2Card*)malloc(sizeof(Sd2Card));//at8M
  SdVolume* sdvolume = (SdVolume*)malloc(sizeof(SdVolume));//at8M~
  sdvolume->root = (SdFile*)malloc(sizeof(SdFile));//at8M~

  Sd2Card* sdcards[3];
  sdcards[0] = (Sd2Card*)malloc(sizeof(Sd2Card));//at eof
  sdcards[1] = (Sd2Card*)malloc(sizeof(Sd2Card));//at eof
  sdcards[2] = (Sd2Card*)malloc(sizeof(Sd2Card));//at eof
  SdVolume* sdvolumes[26];
  int totalVolume = 0;
  SdVolume* currVolume;
  print("Hello from My DOS\r\n");
  printInt((int)sdcards[2]);
  initDisk(sdcards, 0, sdvolumes, &totalVolume);
  initDisk(sdcards, 1, sdvolumes, &totalVolume);
  initDisk(sdcards, 2, sdvolumes, &totalVolume);
  int res;
  for(int i=0;i<totalVolume;i++){
    sdvolumes[i]->root = (SdFile*)malloc(sizeof(SdFile));
    res = sdvolumes[i]->root->openRoot(sdvolumes[i]);
    if(res){
      print("found root");printInt(i);print("\r\n");
      sdvolumes[i]->root->dirList();
    }else{
      print("root error");printInt(i);print("\r\n");
    }
  }
  
  
  SdFile* file = (SdFile*)malloc(sizeof(SdFile));//at8M~
  

  while(1){
    
    char str[10];
    int res;
    print("cmd?\r\n");
    scan(str,-1,-1);
    
    if(equal(str,"i",-1)){
      print("which sd?\r\n");
      int cs = scanInt();
      res = sdcard->init(cs);
      if(res){
        print("sd ok\r\n");

        sdcard->printType();
        print("which partition?\r\n");
        int part = scanInt();
        res = sdvolume->init(sdcard, part);
        if(res){
          print("volume ok\r\n");
          res = sdvolume->root->openRoot(sdvolume);
          currVolume = sdvolume;
          if(res){
            print("root ok\r\n");
          }else{
            print("root ng\r\n");
          }
        }else{
          print("volume ng\r\n");
          printInt(sdvolume->error);print(",");
          printInt(sdvolume->cacheBlockNumber_);
          print("[");
          for(int i=0;i<512;i++){
            printByte(sdvolume->cacheBuffer_.data[i]);
          }
          print("]");
        }
      }else{
        print("sd ng\r\n");
        printInt(sdcard->errorCode());
      }
    }

    if(equal(str,"v",-1)){
      print("which volume?");printInt(totalVolume);print("\r\n");
      int v = scanInt();
      currVolume = sdvolumes[v];
      print("curr volume:");printInt(v);print("\r\n");
    }

    if(equal(str,"dir",-1)){
      currVolume->root->dirList();
    }
    
    if(equal(str,"mkdir",-1)){
      print("dir name?\r\n");
      char filename[12];
      scan(filename,-1,-1);
      res = file->makeDir(currVolume->root, filename);
      if(res){
        print("mkdir ok\r\n");
      }else{
        print("mkdir ng\r\n");
      }

    }
    
    if(equal(str,"load",-1)){
      print("which file?\r\n");
      char filename[12];
      scan(filename,-1,-1);
      res = file->open(currVolume->root, filename, O_READ);
      if(res){
        print("open ok\r\n");
        for(int i=0;i<0x200000;i++){
          int val = file->read();
          ((char*)(0x200000))[i] = val;//at 2Mbyte
          if((i&0x3FF)==0){
            printInt(i);print("\r\n");
          }
        }
      }else{
        print("open ng\r\n");
        printInt(file->fileError);print("\r\n");
      }
    }
    
    if(equal(str,"base",-1)){
      print("base?\r\n");
      int base = scanInt();
      IOWR(VGA, VGA_BASE, base);
      print("done\r\n");
    }

    
    if(equal(str,"draw",-1)){
      for(int j=0;j<768;j++){
        for(int i=0;i<1024;i++){
          if(i==j){
            ((short*)(0x200000))[i+j*2048] = 0xFFFF;//at 2Mbyte
          }
        }
      }
      print("done\r\n");
    }
    
    if(equal(str,"clr",-1)){
      for(int i=0;i<0x80000;i++){
        ((int*)(0x200000))[i] = 0;//at 2Mbyte
      }
      print("done\r\n");
    }

  }
}
