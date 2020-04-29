//中断向量
//浮点数

#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/Sd2Card.cpp"
#include "inc/FileSystem.cpp"



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


short* imgArr;

int drawImg(int blockx, int blocky, int block){
  int basex = 100;
  int basey = 100;
  int blockbase = block*20*20;
  for(int j=0;j<20;j++){
    for(int i=0;i<20;i++){
      int x = basex + blockx * 20 + i;
      int y = basey + blocky * 20 + j;
      ((short*)(0x200000))[x+y*1024] = imgArr[blockbase+i+j*20];//at 2Mbyte
    }
  }
}

int loadImg(SdFile* file, SdVolume* currVolume, char* filename, char* arr){
  int res = file->open(currVolume->root, filename, O_READ);
  if(res){
    print("open ok\r\n");
    print(filename);
    print("\r\n");
    printInt(file->fileSize_);print("\r\n");
    for(int i=0;i<file->fileSize_;i++){
      arr[i] = file->read();
    }
  }else{
    print("open ng\r\n");
    printInt(file->fileError);print("\r\n");
  }
}


int p1(int v1){
  print("a");
  printInt(v1);
}

int p2(int v1){
  print("b");
  printInt(v1);
}

typedef int (*PF)(int);
PF getFunc(int val){
  if(val==1){
    return p1;
  }
  
  if(val==2){
    return p2;
  }
}


int main(){
  
  int (*proc1)(int);

  proc1 = getFunc(1);
  
  proc1(1);
  
  proc1 = getFunc(2);
  
  proc1(1);

  malloc_index = 0;
  imgArr = (short*)malloc(20*20*16*2);

  
  //base + clr
  IOWR(VGA, VGA_BASE, 1024);//1024=2Mbyte
  for(int i=0;i<0x80000;i++){
    ((int*)(0x200000))[i] = 0;//at 2Mbyte
  }
  
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
  printInt(SdVolume::arrayaa[2]);
  if(false){
  initDisk(sdcards, 0, sdvolumes, &totalVolume);
  initDisk(sdcards, 1, sdvolumes, &totalVolume);
  initDisk(sdcards, 2, sdvolumes, &totalVolume);
  }
  sdvolumes[1]->arraybb[2] = 12;
  printInt(sdvolumes[1]->arraybb[2]);

  int res;
  if(false){
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
  }
  
  
  SdFile* file = (SdFile*)malloc(sizeof(SdFile));//at8M~
  
//sd卡提速：
//spi提速
//fat表缓存
//清理 readBlock 512 readData

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
          //printInt(sdvolume->cacheBlockNumber_);
          //print("[");
          //for(int i=0;i<512;i++){
          //  printByte(sdvolume->cacheBuffer_.data[i]);
          //}
          //print("]");
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
    
    if(equal(str,"print",-1)){
      print("which file?\r\n");
      char filename[12];
      scan(filename,-1,-1);
      res = file->open(currVolume->root, filename, O_READ);
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
      IOWR(VGA, VGA_BASE, base);//1024=2Mbyte
      print("done\r\n");
    }
    
    
    if(equal(str,"draw1f",-1)){
      for(int i=0;i<64;i++){
        ((short*)(0x200000))[i+64*1024] = 0xFFFF;//at 2Mbyte
      }
      flushCache(&(((short*)(0x200000))[0+64*1024]));
      print("done\r\n");
    }
    if(equal(str,"draw1",-1)){
      for(int i=0;i<64;i++){
        ((short*)(0x200000))[i+64*1024] = 0xFFFF;//at 2Mbyte
      }
      print("done\r\n");
    }
    if(equal(str,"draw0f",-1)){
      for(int i=0;i<64;i++){
        ((short*)(0x200000))[i+64*1024] = 0;//at 2Mbyte
      }
      flushCache(&(((short*)(0x200000))[0+64*1024]));
      print("done\r\n");
    }
    if(equal(str,"draw0",-1)){
      for(int i=0;i<64;i++){
        ((short*)(0x200000))[i+64*1024] = 0;//at 2Mbyte
      }
      print("done\r\n");
    }

    if(equal(str,"loadimg",-1)){
      loadImg(file, currVolume, "0.img", (char*)(&imgArr[0]));
      loadImg(file, currVolume, "1.img", (char*)(&imgArr[1*20*20]));
      loadImg(file, currVolume, "2.img", (char*)(&imgArr[2*20*20]));
    }
    

    if(equal(str,"draw",-1)){
      print("blockx?\r\n");
      int blockx = scanInt();
      print("blocky?\r\n");
      int blocky = scanInt();
      print("block?\r\n");
      int block = scanInt();

      drawImg(blockx, blocky, block);

      
      //for(int j=0;j<768;j++){
      //  for(int i=0;i<1024;i++){
      //    if(i==j){
      //      ((short*)(0x200000))[i+j*1024] = 0xFFFF;//at 2Mbyte
      //    }
      //  }
      //}
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
