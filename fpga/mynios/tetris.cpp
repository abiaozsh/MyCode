#include "inc/io.h"
#include "inc/system.h"
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
  
  int (*CallBack_DrawNextShape)(Tetris*);
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
			var x = PosX + Public_GetBlockXout;
			var y = PosY - Public_GetBlockYout;
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
  
	int private_getBoard(x, y) {
		return Board[x * 20 + y];
	},
	int private_setBoard(x, y, val) {
		Board[x * 20 + y] = val;
	},

	Public_GetBlockXout;
	Public_GetBlockYout;
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
		CallBack_DrawNextShape(this);
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
	int CallBack_GetRandom = function() {
		return rnd7;//0~6
	};

  int drawImg(int blockx, int blocky, int block){
    
    
  }
  
	int CallBack_DrawNextShape(Tetris* tetris) {
		for (var i = 0; i < 4; i++) {
			for (var j = 0; j < 4; j++) {
				ImgNext[j][i].src = "0.bmp";
			}
		}
		for (var idx = 0; idx < 4; idx++) {
			var block = tetris->Public_GetBlock(tetris->NextShapeNo, 0, idx);
			var i = tetris->Public_GetBlockXout;
			var j = tetris->Public_GetBlockYout;
			ImgNext[j][i].src = block + ".bmp";
		}
	};

	// UI Board
	var SpanBoard = document.getElementById("Board");
	ImgBoard = new Array(20);
	for (var i = 0; i < 20; i++) {
		ImgBoard[i] = new Array(10);
		for (var j = 0; j < 10; j++) {
			img = document.createElement("img");
			img.src = "0.bmp";
			SpanBoard.appendChild(img);
			ImgBoard[i][j] = img;
		}
		var br = document.createElement("br");
		SpanBoard.appendChild(br);
	}

	// UI Next
	var SpanNext = document.getElementById("Next");
	ImgNext = new Array(4);
	for (var i = 0; i < 4; i++) {
		ImgNext[i] = new Array(4);
		for (var j = 0; j < 4; j++) {
			var img = document.createElement("img");
			img.src = "0.bmp";
			SpanNext.appendChild(img);
			ImgNext[i][j] = img;
		}
		var br = document.createElement("br");
		SpanNext.appendChild(br);
	}
	document.onkeydown = keyDown;
	var timename = setInterval("timing();",1000);

	Tetris.Public_Init();
	DrawBoard();
}


function timing() {
	Tetris.Public_Down();
	DrawBoard();
}

function keyDown(key) {
	if (k == space)
		Tetris.Public_Rotate();
	else if (k == d)
		Tetris.Public_Move(1);
	else if (k == a)
		Tetris.Public_Move(-1);
	else if (k == s)
		Tetris.Public_Down();
	else if (k == x) {
		while (Tetris.Public_Down())
			;
	}
	DrawBoard();
}

function DrawBoard() {
	for (var i = 0; i < 10; i++) {
		for (var j = 0; j < 20; j++) {
			ImgBoard[j][i].src = Tetris.Board[i * 20 + 19 - j] + ".bmp";
		}
	}
	for (var idx = 0; idx < 4; idx++) {
		var block = Tetris.Public_GetBlock(Tetris.NowShapeNo, Tetris.NowDirectionNo, idx);
		var x = Tetris.PosX + Tetris.Public_GetBlockXout;
		var y = 19 - (Tetris.PosY - Tetris.Public_GetBlockYout);
		ImgBoard[y][x].src = block + ".bmp";
	}
}

int loadImg(char* filename, char* arr){
  res = file->open(currVolume->root, filename, O_READ);
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


int main()
{
  malloc_index = 0;
  Tetris* tetris = (Tetris*)malloc(sizeof(Tetris));
  Sd2Card* sdcard = (Sd2Card*)malloc(sizeof(Sd2Card));
  SdVolume* sdvolume = (SdVolume*)malloc(sizeof(SdVolume));
  sdvolume->root = (SdFile*)malloc(sizeof(SdFile));
  SdFile* file = (SdFile*)malloc(sizeof(SdFile));


  int cs = 1;
  res = sdcard->init(cs);
  if(res){
    print("sd ok\r\n");
    sdcard->printType();
    print("which partition?\r\n");
    int part = 1;
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
      printInt(sdvolume->error);
    }
  }else{
    print("sd ng\r\n");
    printInt(sdcard->errorCode());
  }

  loadImg("0.img", img0);
  loadImg("1.img", img0);
  loadImg("2.img", img0);

	print("Hello from tetris!\r\n");

  while(1){
    IOWR(MYTIMER, 0, 0);
    while(1)
    {
      rnd7++;
      if(rnd7==7){
        rnd7 = 0;
      }
      int time = IORD(MYTIMER, 0);
      if(time > 1000000){
        time<=0;
        timing();
      }
      int tmp = IORD(MYUART, 0);
      if(tmp&0x100){
        int key = tmp & 0xFF;
        keyDown(key);
      }

      
    }
    
    
  }
  return 0;
}
