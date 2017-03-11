"use strict";
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

var Tetris = {
	_block : new Array(//
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
	),
	_chg : new Array(0x000, 0x011, 0x045, 0x033, 0x041, 0x015, 0x066, 0x077, 0x068, 0x097, 0x0A6, 0x07B, 0x0C3, 0x09B, 0x03E, 0x0A8),
	Board : new Array(200),
	PosX : 0,
	PosY : 0,
	NextShapeNo : 0,
	NowShapeNo : 0,
	NowDirectionNo : 0,
	CallBack_GetRandom : function() {
	},
	CallBack_DrawNextShape : function() {
	},
	Public_Init : function() {
		// init Board
		this.Board = new Array(200);
		this.private_Clear();
		this.private_NextShape();
		this.private_NextShape();
	},
	Public_Down : function() {
		this.PosY--;
		if (this.private_isTouch()) {
			this.PosY++;
			this.private_touchDo();
			return false;
		} else {
			return true;
		}
	},
	Public_Move : function(n) {
		this.PosX += n;
		if (this.private_isTouch()) {
			this.PosX -= n;
		}
	},
	Public_Rotate : function() {
		this.NowDirectionNo = (this.NowDirectionNo + 1) & 3;
		if (this.private_isTouch()) {
			this.PosX--;
			if (this.private_isTouch()) {
				this.PosX--;
				if (this.private_isTouch()) {
					this.PosX--;
					if (this.private_isTouch()) {
						this.PosX += 3;
						this.NowDirectionNo = (this.NowDirectionNo + 3) & 3;
					}
				}
			}
		}
	},
	private_touchDo : function() {
		// add to board
		for (var idx = 0; idx < 4; idx++) {
			var block = this.Public_GetBlock(this.NowShapeNo, this.NowDirectionNo, idx);
			var x = this.PosX + this.Public_GetBlockXout;
			var y = this.PosY - this.Public_GetBlockYout;
			this.private_setBoard(x, y, block);
		}

		// clear line
		for (var j = 19; j >= 0; j--) {
			var line = 1;
			for (var i = 0; i < 10; i++) {
				if (this.private_getBoard(i, j) == 0) {
					line = 0;
					break;
				}
			}
			if (line == 1) {
				for (var k = 0; k < 10; k++) {
					if (j - 1 >= 0) {
						var temp = this.private_getBoard(k, j - 1);
						temp = this._chg[temp] & 0x0F;
						this.private_setBoard(k, j - 1, temp);
					}
					if (j + 1 < 20) {
						var temp = this.private_getBoard(k, j + 1);
						temp = (this._chg[temp] >> 4) & 0x0F;
						this.private_setBoard(k, j + 1, temp);
					}
					this.private_setBoard(k, j, 0);
				}
				for (var k = j; k < 19; k++) {
					for (var l = 0; l < 10; l++) {
						this.private_setBoard(l, k, this.private_getBoard(l, k + 1));
					}
				}
			}
		}

		// is gameover
		for (var i = 0; i < 10; i++) {
			if (this.private_getBoard(i, 18) != 0) {
				this.private_Clear();
				break;
			}
		}
		this.private_NextShape();
	},
	private_getBoard : function(x, y) {
		return this.Board[x * 20 + y];
	},
	private_setBoard : function(x, y, val) {
		this.Board[x * 20 + y] = val;
	},

	Public_GetBlockXout : 0,
	Public_GetBlockYout : 0,
	Public_GetBlock : function(ShapeNo, DirectionNo, idx) {
		var dat = this._block[(ShapeNo << 4) + (DirectionNo << 2) + idx];
		this.Public_GetBlockYout = dat & 3;// (0000 00XX)
		dat >>= 2;
		this.Public_GetBlockXout = dat & 3;// (0000 XX00)
		dat >>= 2;
		return dat;// (xxxx 0000)
	},
	private_isTouch : function() {
		for (var idx = 0; idx < 4; idx++) {
			var block = this.Public_GetBlock(this.NowShapeNo, this.NowDirectionNo, idx);
			var x = this.PosX + this.Public_GetBlockXout;
			var y = this.PosY - this.Public_GetBlockYout;
			if (y < 0 || y > 19 || x < 0 || x > 9) {
				return true;
			}
			if (this.private_getBoard(x, y) != 0) {
				return true;
			}
		}
		return false;
	},
	private_NextShape : function() {
		this.NowShapeNo = this.NextShapeNo;
		this.NextShapeNo = this.CallBack_GetRandom();
		this.NowDirectionNo = 0;
		this.PosY = 19;
		this.PosX = 3;
		this.CallBack_DrawNextShape();
	},
	private_Clear : function() {
		for (var i = 0; i < 10; i++) {
			for (var j = 0; j < 20; j++) {
				this.private_setBoard(i, j, 0);
			}
		}
	}
};

var ImgBoard;
var ImgNext;

// 从上到下，从左到右，先低后高

function __OnLoad() {
	Tetris.CallBack_GetRandom = function() {
		return Math.floor(Math.random() * 7);
	};

	Tetris.CallBack_DrawNextShape = function() {
		for (var i = 0; i < 4; i++) {
			for (var j = 0; j < 4; j++) {
				ImgNext[j][i].src = "0.bmp";
			}
		}
		for (var idx = 0; idx < 4; idx++) {
			var block = Tetris.Public_GetBlock(Tetris.NextShapeNo, 0, idx);
			var i = Tetris.Public_GetBlockXout;
			var j = Tetris.Public_GetBlockYout;
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

function keyDown(event) {
	var e = event || window.event || arguments.callee.caller.arguments[0];
	var k = e.keyCode;
	if (k == 32)
		Tetris.Public_Rotate();
	else if (k == 39)
		Tetris.Public_Move(1);
	else if (k == 37)
		Tetris.Public_Move(-1);
	else if (k == 38)
		Tetris.Public_Down();
	else if (k == 40) {
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
