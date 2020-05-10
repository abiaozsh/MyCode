//函数指针
//变量指针
//中断向量
//浮点数

//sd硬件层用 E:\MyCode.github\arduino\softSD\simple Nofat  （带mmc）（现有的）
//文件系统用 E:\MyCode.github\fpga\mynios\inc\SD

//#define DEBUG

#include "inc/io.h"
#include "inc/system.h"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/sd.cpp"




int main()
{
  
  SDcard* sdcard;
  SdVolume* sdvolume;
  Root* root;
  File* file;
  
  sdcard = (SDcard*)(0x1000);//at sdram [512]
  sdvolume = (SdVolume*)(0x2000);
  root = (Root*)(0x3000);
  file = (File*)(0x4000);
  
  sdcard->chip_select = 0;
  sdcard->inBlock = 0;
  
	char sbuff[20];

	print("Hello from Nios II!\r\n");

  //short* shorttest = (short*)(0x2000);
  //shorttest[0] = 12;
  //shorttest[1] = 34;
  //shorttest[2] = 56;
  //shorttest[3] = 78;
  //shorttest[4] = 90;
  //shorttest[5] = 12345;
  //shorttest[6] = 65432;
  //
  //printInt(shorttest[6]);print("\r\n");
  //printInt(shorttest[5]);print("\r\n");
  //printInt(shorttest[4]);print("\r\n");
  //printInt(shorttest[3]);print("\r\n");
  //printInt(shorttest[2]);print("\r\n");
  //printInt(shorttest[1]);print("\r\n");
  //printInt(shorttest[0]);print("\r\n");
  
  
	while(1){
    scan(sbuff,-1,-1);

    
    
		if(equal(sbuff,"i",-1)){
      print("which sd?:\r\n");
      
      sdcard->chip_select = scanInt();
      printInt(sdcard->chip_select);
			print("init start!\r\n");
			int result = MMCCard_cardinit(sdcard);
      if(result){
        print("init success,initSdVolume\r\n");
        result = SdVolume_volumeinit(sdvolume, sdcard, 1);
        if(result){
          print("initSdVolume success!\r\n");
          
          //print("cacheDirty       =");printInt(sdvolume->cacheDirty        );print("\r\n");
          //print("cacheBlockNumber =");printInt(sdvolume->cacheBlockNumber  );print("\r\n");
          //print("cacheMirrorBlock =");printInt(sdvolume->cacheMirrorBlock  );print("\r\n");
          //print("fatCount         =");printInt(sdvolume->fatCount          );print("\r\n");
          //print("blocksPerCluster =");printInt(sdvolume->blocksPerCluster  );print("\r\n");
          //print("allocSearchStart =");printInt(sdvolume->allocSearchStart  );print("\r\n");
          //print("blocksPerFat     =");printInt(sdvolume->blocksPerFat      );print("\r\n");
          //print("clusterCount     =");printInt(sdvolume->clusterCount      );print("\r\n");
          //print("clusterSizeShift =");printInt(sdvolume->clusterSizeShift  );print("\r\n");
          //print("dataStartBlock   =");printInt(sdvolume->dataStartBlock    );print("\r\n");
          //print("fatStartBlock    =");printInt(sdvolume->fatStartBlock     );print("\r\n");
          //print("fatType          =");printInt(sdvolume->fatType           );print("\r\n");
          //print("rootDirEntryCount=");printInt(sdvolume->rootDirEntryCount );print("\r\n");
          //print("rootDirStart     =");printInt(sdvolume->rootDirStart      );print("\r\n");
          
          
          Root_openRoot(root, sdvolume);
          print(root->volumeLabel,11);
          
        }else{
          print("initSdVolume fail!\r\n");
          //result = SdVolume_volumeinit(sdvolume, 1);
        }

      }else{
        print("init fail!\r\n");
      }
		}

    if(equal(sbuff,"r",-1)){
      print("file?:\r\n");
      scan(sbuff,-1,-1);
      print(sbuff);
      int result = File_open(root, file, sbuff);
      if(result){
        print("[");
        int i;
        for(i=0;i<4096;i++){
          if((i&15)==0 && i!=0){
            print("\n");
          }
          int data = File_read(file);
          //printByte(data);
          uart_write(data);
          if(data==-1){
            break;
          }
        }
        print("]");
      }else{
        print("open error\r\n");
      }
    }


	}
  return 0;
}
