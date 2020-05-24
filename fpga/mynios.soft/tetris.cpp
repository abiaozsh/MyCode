#include "inc/io.h"
#include "inc/system.h"
#include "inc/system.cpp"
#include "inc/uart.cpp"
#include "inc/print.cpp"
#include "inc/spi.cpp"
#include "inc/Sd2Card.cpp"
#include "inc/FileSystem.cpp"

//"use strict";
// need
// 8位计算能力 + - 移位
// 3k rom 512 ram
// keyboard/joystick input
// display
// random
// timer
// 00 01 02 03
// 10 11 12 13
// 20 21 22 23
// 30 31 32 33
// 19-0 .. 19-9
// .. .. ..
// 0-0 .. 0-9

int _block[] = {//
	0x070, 0x034, 0x038, 0x06C,//
	0x050, 0x021, 0x022, 0x043,//
	0x070, 0x034, 0x038, 0x06C,//
	0x050, 0x021, 0x022, 0x043,// ;1 长条
	0x0B0, 0x091, 0x084, 0x0A5,//
	0x0B0, 0x091, 0x084, 0x0A5,//
	0x0B0, 0x091, 0x084, 0x0A5,//
	0x0B0, 0x091, 0x084, 0x0A5,// ;2 方
	0x071, 0x0B4, 0x0A5, 0x068,//
	0x050, 0x091, 0x085, 0x046,//
	0x071, 0x0B4, 0x0A5, 0x068,//
	0x050, 0x091, 0x085, 0x046,// ;3 Z
	0x070, 0x084, 0x095, 0x069,//
	0x0B1, 0x042, 0x054, 0x0A5,//
	0x070, 0x084, 0x095, 0x069,//
	0x0B1, 0x042, 0x054, 0x0A5,// ;4 反Z
	0x070, 0x034, 0x088, 0x049,//
	0x072, 0x054, 0x025, 0x0A6,//
	0x050, 0x091, 0x035, 0x069,//
	0x0B0, 0x021, 0x042, 0x064,// ;5 反L
	0x0B0, 0x041, 0x034, 0x068,//
	0x070, 0x084, 0x025, 0x046,//
	0x071, 0x035, 0x058, 0x0A9,//
	0x050, 0x021, 0x092, 0x066,// ;6 L
	0x070, 0x0E4, 0x045, 0x068,//
	0x071, 0x054, 0x0F5, 0x046,//
	0x071, 0x054, 0x0C5, 0x069,//
	0x050, 0x0D1, 0x042, 0x065 // ;7 T
	};
int _chg[] = {0x000, 0x011, 0x045, 0x033, 0x041, 0x015, 0x066, 0x077, 0x068, 0x097, 0x0A6, 0x07B, 0x0C3, 0x09B, 0x03E, 0x0A8};

class Tetris {
  public:
	int Board[200];
	int PosX;
	int PosY;
	int NextShapeNo;
	int NowShapeNo;
	int NowDirectionNo;

  int (*CallBack_GetRandom)(void);
	//int CallBack_GetRandom(){
  //  return *rnd7;
	//};
  
  //int (*CallBack_DrawNextShape)(Tetris*);
	//int CallBack_DrawNextShape() {
  //  call GUI
	//},

	int Public_Init() {
		private_Clear();
		private_NextShape();
		private_NextShape();
	};
  
	int Public_Down() {
		PosY--;
		if (private_isTouch()) {
			PosY++;
			private_touchDo();
			return false;
		} else {
			return true;
		}
	};
  
	int Public_Move(int n) {
		PosX += n;
		if (private_isTouch()) {
			PosX -= n;
		}
	};
  
	int Public_Rotate() {
		NowDirectionNo = (NowDirectionNo + 1) & 3;
		if (private_isTouch()) {
			PosX--;
			if (private_isTouch()) {
				PosX--;
				if (private_isTouch()) {
					PosX--;
					if (private_isTouch()) {
						PosX += 3;
						NowDirectionNo = (NowDirectionNo + 3) & 3;
					}
				}
			}
		}
	};
  
	int private_touchDo() {
		// add to board
		for (int idx = 0; idx < 4; idx++) {
			int block = Public_GetBlock(NowShapeNo, NowDirectionNo, idx);
			int x = PosX + Public_GetBlockXout;
			int y = PosY - Public_GetBlockYout;
			private_setBoard(x, y, block);
		}

		// clear line
		for (int j = 19; j >= 0; j--) {
			int line = 1;
			for (int i = 0; i < 10; i++) {
				if (private_getBoard(i, j) == 0) {
					line = 0;
					break;
				}
			}
			if (line == 1) {
				for (int k = 0; k < 10; k++) {
					if (j - 1 >= 0) {
						int temp = private_getBoard(k, j - 1);
						temp = _chg[temp] & 0x0F;
						private_setBoard(k, j - 1, temp);
					}
					if (j + 1 < 20) {
						int temp = private_getBoard(k, j + 1);
						temp = (_chg[temp] >> 4) & 0x0F;
						private_setBoard(k, j + 1, temp);
					}
					private_setBoard(k, j, 0);
				}
				for (int k = j; k < 19; k++) {
					for (int l = 0; l < 10; l++) {
						private_setBoard(l, k, private_getBoard(l, k + 1));
					}
				}
			}
		}

		// is gameover
		for (int i = 0; i < 10; i++) {
			if (private_getBoard(i, 18) != 0) {
				private_Clear();
				break;
			}
		}
		private_NextShape();
	};
  
	int private_getBoard(int x, int y) {
		return Board[x + y * 10];
	};
  
	int private_setBoard(int x, int y, int val) {
		Board[x + y * 10] = val;
	};

	int Public_GetBlockXout;
	int Public_GetBlockYout;
	int Public_GetBlock(int ShapeNo, int DirectionNo, int idx) {
		int dat = _block[(ShapeNo << 4) + (DirectionNo << 2) + idx];
		Public_GetBlockYout = dat & 3;// (0000 00XX)
		dat >>= 2;
		Public_GetBlockXout = dat & 3;// (0000 XX00)
		dat >>= 2;
		return dat;// (xxxx 0000)
	};
  
	int private_isTouch() {
		for (int idx = 0; idx < 4; idx++) {
			int block = Public_GetBlock(NowShapeNo, NowDirectionNo, idx);
			int x = PosX + Public_GetBlockXout;
			int y = PosY - Public_GetBlockYout;
			if (y < 0 || y > 19 || x < 0 || x > 9) {
				return true;
			}
			if (private_getBoard(x, y) != 0) {
				return true;
			}
		}
		return false;
	};
  
	int private_NextShape() {
		NowShapeNo = NextShapeNo;
		NextShapeNo = CallBack_GetRandom();
		NowDirectionNo = 0;
		PosY = 19;
		PosX = 3;
	};
  
	int private_Clear() {
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 20; j++) {
				private_setBoard(i, j, 0);
			}
		}
	}
};

int rnd7;
short* imgArr;
Tetris* tetris;

int CallBack_GetRandom() {
  return rnd7;//0~6
};

int currBuff;
int buffAddr;
int drawImg(int blockx, int blocky, short* block, int isNext){
  int basex = 50;
  int basey = 100;
  if(isNext){
    basex = 200;
  }
  basex = basex + blockx * 10;
  basey = basey + blocky * 20;
  int* src = (int*)block;
  for(int j=0;j<20;j++){
    int j_temp = j*10;
    for(int i=0;i<10;i++){
      int x = basex + i;
      int y = basey + j;
      ((int*)(buffAddr))[x+(y<<9)] = src[i+j_temp];//at 2Mbyte
    }
  }
}
//int drawImg(int blockx, int blocky, short* block, int isNext){
//  int basex = 100;
//  int basey = 100;
//  if(isNext){
//    basex = 500;
//  }
//  basex = basex + blockx * 20;
//  basey = basey + blocky * 20;
//  for(int j=0;j<20;j++){
//    int j_temp = j*20;
//    for(int i=0;i<20;i++){
//      int x = basex + i;
//      int y = basey + j;
//      ((short*)(buffAddr))[x+(y<<10)] = block[i+j_temp];//at 2Mbyte
//    }
//  }
//}


int* lastBoard;
int* currBoard;
int* lastNext;
int* currNext;
void InitBoard() {
  lastBoard = (int*)malloc(200*4*2);
  currBoard = (int*)malloc(200*4);
  
  lastNext = (int*)malloc(16*4*2);
  currNext = (int*)malloc(16*4);
  for(int i=0;i<200*2;i++){
    lastBoard[i] = -1;
  }
  for(int i=0;i<16*2;i++){
    lastNext[i] = -1;
  }
}

void DrawBoard() {
  if(currBuff==0){
    buffAddr = 0x200000;
    currBuff = 1;
  }else{
    buffAddr = 0x400000;
    currBuff = 0;
  }

  for (int j = 0; j < 20; j++) {
    for (int i = 0; i < 10; i++) {
      //ImgBoard[j][i].src = Tetris.Board[i * 20 + 19 - j] + ".bmp";
      int block = tetris->Board[i + (19 - j) * 10];
      currBoard[i+j*10] = block;
    }
  }
  
  for (int idx = 0; idx < 4; idx++) {
    int block = tetris->Public_GetBlock(tetris->NowShapeNo, tetris->NowDirectionNo, idx);
    int x = tetris->PosX + tetris->Public_GetBlockXout;
    int y = 19 - (tetris->PosY - tetris->Public_GetBlockYout);
    //ImgBoard[y][x].src = block + ".bmp";
    currBoard[x+y*10] = block;
  }
  
  int* lb = lastBoard;
  if(currBuff){
    lb+=200;
  }
  for (int j = 0; j < 20; j++) {
    for (int i = 0; i < 10; i++) {
      if(lb[i+j*10]!=currBoard[i+j*10]){
        lb[i+j*10] = currBoard[i+j*10];
        int block = lb[i+j*10];
        drawImg(i,j,&(imgArr[block*20*20]), false);
      }
    }
  }

  
  
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      //ImgNext[j][i].src = "0.bmp";
      currNext[i+j*4] = 0;
    }
  }
  for (int idx = 0; idx < 4; idx++) {
    int block = tetris->Public_GetBlock(tetris->NextShapeNo, 0, idx);
    int i = tetris->Public_GetBlockXout;
    int j = tetris->Public_GetBlockYout;
    //ImgNext[j][i].src = block + ".bmp";
    currNext[i+j*4] = block;
  }
  
  int* ln = lastNext;
  if(currBuff){
    ln+=16;
  }
  for (int j = 0; j < 4; j++) {
    for (int i = 0; i < 4; i++) {
      if(ln[i+j*4]!=currNext[i+j*4]){
        int block = ln[i+j*4] = currNext[i+j*4];
        drawImg(i,j,&(imgArr[block*20*20]), true);
      }
    }
  }

  if(currBuff){
    IOWR(VGA, VGA_BASE, 1024);
  }else{
    IOWR(VGA, VGA_BASE, 2048);
  }
}

void timing() {
	tetris->Public_Down();
	DrawBoard();
}

void keyDown(int k) {
	if (k == ' '){
		tetris->Public_Rotate();
	}else if (k == 'd'){
		tetris->Public_Move(1);
	}else if (k == 'a'){
		tetris->Public_Move(-1);
	}else if (k == 's'){
		tetris->Public_Down();
	}else if (k == 'x') {
		while (tetris->Public_Down())
			;
	}
	DrawBoard();
  uart_write(k);
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
    print(filename);print(" open ng\r\n");
    printInt(file->fileError);print("\r\n");
  }
}
int screenInit2(int screenBase){
  IOWR(VGA, VGA_BASE, screenBase);//1024=2Mbyte
  screen_base = screenBase*2048;
  for(int i=0;i<0x80000;i++){
    ((int*)(screen_base))[i] = 0;//at 2Mbyte
  }
}

int main()
{
  malloc_index = 0;
  print("Hello from tetris!\r\n");

  imgArr = (short*)malloc(20*20*16*2);
  
  tetris = (Tetris*)malloc(sizeof(Tetris));
  Sd2Card* sdcard = (Sd2Card*)malloc(sizeof(Sd2Card));
  SdVolume* sdvolume = (SdVolume*)malloc(sizeof(SdVolume));
  sdvolume->root = (SdFile*)malloc(sizeof(SdFile));
  SdFile* file = (SdFile*)malloc(sizeof(SdFile));

  InitBoard();
  
  tetris->CallBack_GetRandom = CallBack_GetRandom;
  
  //tetris->CallBack_DrawNextShape = CallBack_DrawNextShape;

  int cs = 0;////////////////////////////////
  int res = sdcard->init(cs);
  if(res){
    print("sd ok\r\n");
    sdcard->printType();
    print("which partition?\r\n");
    int part = 1;//////////////////////////////////
    res = sdvolume->init(sdcard, part);
    if(res){
      print("volume ok\r\n");
      res = sdvolume->root->openRoot(sdvolume);

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

  if(res){
    loadImg(file, sdvolume, "0.img" , (char*)(&imgArr[0]));
    loadImg(file, sdvolume, "1.img" , (char*)(&imgArr[1*20*20]));
    loadImg(file, sdvolume, "2.img" , (char*)(&imgArr[2*20*20]));
    loadImg(file, sdvolume, "3.img" , (char*)(&imgArr[3*20*20]));
    loadImg(file, sdvolume, "4.img" , (char*)(&imgArr[4*20*20]));
    loadImg(file, sdvolume, "5.img" , (char*)(&imgArr[5*20*20]));
    loadImg(file, sdvolume, "6.img" , (char*)(&imgArr[6*20*20]));
    loadImg(file, sdvolume, "7.img" , (char*)(&imgArr[7*20*20]));
    loadImg(file, sdvolume, "8.img" , (char*)(&imgArr[8*20*20]));
    loadImg(file, sdvolume, "9.img" , (char*)(&imgArr[9*20*20]));
    loadImg(file, sdvolume, "10.img", (char*)(&imgArr[10*20*20]));
    loadImg(file, sdvolume, "11.img", (char*)(&imgArr[11*20*20]));
    loadImg(file, sdvolume, "12.img", (char*)(&imgArr[12*20*20]));
    loadImg(file, sdvolume, "13.img", (char*)(&imgArr[13*20*20]));
    loadImg(file, sdvolume, "14.img", (char*)(&imgArr[14*20*20]));
    loadImg(file, sdvolume, "15.img", (char*)(&imgArr[15*20*20]));
    
    screenInit2(1024);
    screenInit2(2048);

    tetris->Public_Init();
    DrawBoard();

    IOWR(MYTIMER, 0, 0);
    int skip = 0;
    while(1)
    {
      rnd7++;
      if(rnd7==7){
        rnd7 = 0;
      }
      int time = IORD(MYTIMER, 0);
      if(time > 1000000){
        IOWR(MYTIMER, 0, 0);
        timing();
      }
      int tmp = IORD(MYUART, 0);
      if(tmp&0x100){
        int key = tmp & 0xFF;
        uart_write(key);
        keyDown(key);
      }
      
      tmp = IORD(MYKEYB, 0);
      if(tmp&0x400){
        if(skip){
          skip = 0;
        }else{
          tmp = (tmp>>1) & 0xFF;
          if(tmp == 0xF0 || tmp ==0xE0)
          {
            skip = 1;
          }
          else{
            keyDown(PS2Keymap_US[tmp]);
            IORD(MYKEYB, 0);
          }
        }
      }

      
    }
    
  }else{
    while(1);
  }
  return 0;
}
