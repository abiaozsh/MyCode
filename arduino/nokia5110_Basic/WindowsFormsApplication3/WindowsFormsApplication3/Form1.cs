﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication3
{
	public partial class Form1 : Form
	{
		int[] buff = {
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xF8, 0xFC, 0xFC,   // 0x0010 (16) pixels
//0xFE, 0xFE, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0x7F, 0xFE, 0xFE, 0xFE,   // 0x0020 (32) pixels
//0xFC, 0xFC, 0xF8, 0xF0, 0xF0, 0xE0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0,   // 0x0030 (48) pixels
//0xF0, 0xF0, 0xF8, 0xFC, 0xFC, 0xFE, 0xFE, 0xFE, 0x7F, 0x7F, 0x7F, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F,   // 0x0040 (64) pixels
//0x7F, 0x7F, 0x7F, 0xFE, 0xFE, 0xFC, 0xFC, 0xF8, 0xF8, 0xF0, 0xE0, 0xC0, 0x86, 0x06, 0x06, 0x00,   // 0x0050 (80) pixels
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xFC, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0060 (96) pixels
//0x1F, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,   // 0x0070 (112) pixels
//0x80, 0x80, 0x00, 0x00, 0x01, 0x01, 0x03, 0x0F, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xFE,   // 0x0080 (128) pixels
//0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F, 0x03, 0x01, 0x01, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0xF8,   // 0x0090 (144) pixels
//0xF8, 0xF8, 0xF8, 0x80, 0x80, 0x80, 0x00, 0x00, 0x00, 0x00, 0x01, 0x07, 0x1F, 0xFF, 0xFF, 0xFF,   // 0x00A0 (160) pixels
//0xFF, 0xFF, 0xFC, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x7F, 0xFF,   // 0x00B0 (176) pixels
//0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x07, 0x07, 0x07, 0x07,   // 0x00C0 (192) pixels
//0x07, 0x07, 0x07, 0x07, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xF0, 0xF8, 0xFE, 0xFF,   // 0x00D0 (208) pixels
//0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xF8, 0xF0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x00, 0x07,   // 0x00E0 (224) pixels
//0x07, 0x07, 0x07, 0x3F, 0x3F, 0x3F, 0x3F, 0x07, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0,   // 0x00F0 (240) pixels
//0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0100 (256) pixels
//0x00, 0x00, 0x00, 0x00, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFE, 0xFE, 0xFC, 0xFC,   // 0x0110 (272) pixels
//0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFC, 0xFE, 0xFF, 0x7F, 0x7F, 0x3F, 0x3F,   // 0x0120 (288) pixels
//0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x01, 0x03, 0x07, 0x0F, 0x1F, 0x3F, 0x3F, 0x7F,   // 0x0130 (304) pixels
//0x7F, 0xFF, 0xFE, 0xFC, 0xFC, 0xFC, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xF8, 0xFC, 0xFC, 0xFE,   // 0x0140 (320) pixels
//0xFE, 0xFF, 0x7F, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,   // 0x0150 (336) pixels
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0xC0, 0x80, 0x00, 0x00,   // 0x0160 (352) pixels
//0x00, 0x00, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0xC1, 0x81, 0x81, 0x01, 0x01, 0xC1, 0xC1, 0xC0, 0xC0,   // 0x0170 (368) pixels
//0xC0, 0xC0, 0x80, 0x00, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x80, 0x00,   // 0x0180 (384) pixels
//0x00, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC0, 0xC1, 0xC1, 0x01, 0x01, 0xC1, 0xC1, 0xC1, 0x01, 0x01,   // 0x0190 (400) pixels
//0x01, 0xC1, 0xC1, 0x01, 0x00, 0x00, 0x80, 0xC0, 0xC0, 0xC0, 0xC0, 0x80, 0x80, 0x00, 0x00, 0x00,   // 0x01A0 (416) pixels
//0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0xF8, 0xFF, 0x3F, 0x3B,   // 0x01B0 (432) pixels
//0x3F, 0x7F, 0xFE, 0xF0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0x1C, 0x3C, 0xFF, 0xFF, 0xC7, 0x00, 0x00,   // 0x01C0 (448) pixels
//0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xF1, 0x7F, 0x3F, 0x1F, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xC0, 0xE0,   // 0x01D0 (464) pixels
//0xFF, 0x7F, 0x3F, 0x00, 0x00, 0xE0, 0xE0, 0xFF, 0xFF, 0xFF, 0xE0, 0xE0, 0xC0, 0x00, 0x00, 0xFF,   // 0x01E0 (480) pixels
//0xFF, 0x07, 0x0F, 0x3E, 0xFC, 0xFF, 0xFF, 0x00, 0x00, 0x3F, 0x7F, 0xFF, 0xE0, 0xC0, 0xE1, 0x7F,   // 0x01F0 (496) pixels
//0x7F, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x9F, 0xFF, 0x3F, 0x9F, 0x3F, 0xFF,   // 0x0010 (16) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0020 (32) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0030 (48) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0040 (64) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0050 (80) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3C, 0x99, 0xFC,   // 0x0060 (96) pixels
0x3F, 0x99, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x3F, 0x3F, 0x3F, 0x7F, 0x7F, 0xFF,   // 0x0070 (112) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0x9F, 0x9F, 0x3F, 0x3F, 0x7F, 0xFF, 0xFF,   // 0x0080 (128) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0090 (144) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x99, 0x99, 0x99, 0x99, 0xC3, 0xF3, 0xE7, 0x0F, 0xFF, 0xFF, 0xFF,   // 0x00A0 (160) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x00B0 (176) pixels
0xFF, 0x3C, 0x99, 0x1C, 0xFF, 0x99, 0x3C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF3, 0x73, 0x73,   // 0x00C0 (192) pixels
0x73, 0x78, 0x7E, 0xFC, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x79, 0x39, 0x39, 0xF9, 0x3C,   // 0x00D0 (208) pixels
0x7F, 0xFE, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x1F, 0xCF, 0xCF, 0xCF, 0xCF, 0x1F,   // 0x00E0 (224) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xF3, 0x83, 0x33, 0x33, 0x89, 0xFF,   // 0x00F0 (240) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0100 (256) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x9C, 0x89, 0x48, 0xF9, 0xF9, 0x7C, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0110 (272) pixels
0xFF, 0xE0, 0x66, 0x66, 0x66, 0x66, 0x26, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x98,   // 0x0120 (288) pixels
0x33, 0x70, 0x33, 0x93, 0x98, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x0E, 0x7C,   // 0x0130 (304) pixels
0x7C, 0x7C, 0x4C, 0x1E, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x87, 0xE7, 0xE7, 0xE7, 0xE7, 0xE7,   // 0x0140 (320) pixels
0xE7, 0xE7, 0x81, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0150 (336) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x18, 0xCC, 0xCC, 0xCC, 0xCC, 0x18, 0xFF,   // 0x0160 (352) pixels
0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xFE, 0xF0, 0xE6, 0xE6, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0170 (368) pixels
0xF1, 0xF9, 0xF9, 0xF9, 0xF9, 0xF8, 0xF9, 0xF9, 0xF1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0180 (384) pixels
0xFF, 0xFE, 0xFC, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x0190 (400) pixels
0xFF, 0x23, 0x99, 0x99, 0x99, 0x99, 0x83, 0x9F, 0x0F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01A0 (416) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF9, 0xFC, 0xFC,   // 0x01B0 (432) pixels
0xFC, 0xFC, 0xFC, 0xFC, 0xF8, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01C0 (448) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01D0 (464) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01E0 (480) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,   // 0x01F0 (496) pixels
0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 
		};



		public Form1()
		{
			InitializeComponent();
		}


		//x:84
		//y:48
		private void setPixel(int x, int y, int val)
		{
			int idx = x + (y >> 3) * 84;
			int shift = y & 7;
			if (val == 0)
			{
				buff[idx] &= ~(1 << shift);
			}
			else
			{
				buff[idx] |= (1 << shift);
			}
		}











		byte[] Board = new byte[200];//int[]
		byte PosX = 0;
		byte PosY = 0; //int

		byte NextShapeNum = 0;//int
		byte NowShapeNum = 0;//int

		byte NowShapeRotateIdx = 0;

		byte[] _Block = {
  0x007,0x000,0x003,0x000,0x003,0x000,0x006,0x000, 0x025,0x042,0x000,0x000,0x000,0x000,0x000,0x000, //;0
  0x007,0x000,0x003,0x000,0x003,0x000,0x006,0x000, 0x025,0x042,0x000,0x000,0x000,0x000,0x000,0x000, //;1 1
  0x09B,0x000,0x0A8,0x000,0x000,0x000,0x000,0x000, 0x09B,0x000,0x0A8,0x000,0x000,0x000,0x000,0x000, //;2
  0x09B,0x000,0x0A8,0x000,0x000,0x000,0x000,0x000, 0x09B,0x000,0x0A8,0x000,0x000,0x000,0x000,0x000, //;3 2
  0x070,0x000,0x0AB,0x000,0x006,0x000,0x000,0x000, 0x095,0x000,0x080,0x004,0x000,0x000,0x000,0x000, //;4
  0x070,0x000,0x0AB,0x000,0x006,0x000,0x000,0x000, 0x095,0x000,0x080,0x004,0x000,0x000,0x000,0x000, //;5 3
  0x007,0x000,0x098,0x000,0x060,0x000,0x000,0x000, 0x0B0,0x004,0x0A5,0x000,0x000,0x000,0x000,0x000, //;6
  0x007,0x000,0x098,0x000,0x060,0x000,0x000,0x000, 0x0B0,0x004,0x0A5,0x000,0x000,0x000,0x000,0x000, //;7 4
  0x007,0x000,0x003,0x000,0x048,0x000,0x000,0x000, 0x000,0x007,0x025,0x00A,0x000,0x000,0x000,0x000, //;8
  0x095,0x000,0x030,0x000,0x060,0x000,0x000,0x000, 0x02B,0x004,0x006,0x000,0x000,0x000,0x000,0x000, //;9 5
  0x04B,0x000,0x003,0x000,0x006,0x000,0x000,0x000, 0x007,0x000,0x028,0x004,0x000,0x000,0x000,0x000, //;A
  0x070,0x000,0x030,0x000,0x0A5,0x000,0x000,0x000, 0x025,0x009,0x000,0x006,0x000,0x000,0x000,0x000, //;B 6
  0x007,0x000,0x04E,0x000,0x006,0x000,0x000,0x000, 0x070,0x000,0x0F5,0x004,0x000,0x000,0x000,0x000, //;C
  0x070,0x000,0x0C5,0x000,0x060,0x000,0x000,0x000, 0x0D5,0x004,0x060,0x000,0x000,0x000,0x000,0x000  //;D 7
};

		byte[] _Bottom = {
  0x011,0x011,0x004,0x000,0x011,0x011,0x004,0x000, 0x022,0x000,0x022,0x000,0x022,0x000,0x022,0x000, //E 1 2
  0x022,0x001,0x032,0x000,0x022,0x001,0x032,0x000, 0x021,0x002,0x023,0x000,0x021,0x002,0x023,0x000, //F 3 4
  0x011,0x002,0x033,0x000,0x022,0x002,0x013,0x000, 0x012,0x001,0x031,0x000,0x022,0x002,0x033,0x000, //0 5 6
  0x021,0x001,0x032,0x000,0x022,0x002,0x023,0x000                                                   //1 7
};
		byte[] _Left = {
  0x014,0x014,0x022,0x022,0x023,0x023,0x023,0x023, 0x023,0x023,0x023,0x023,0x023,0x023              //2 1 2 3 4 5 6 7
};

		//01531567876B3BE8
		byte[] _Conv1 = {
  0x000,0x001,0x005,0x003,0x001,0x005,0x006,0x007, 0x008,0x007,0x006,0x00B,0x003,0x00B,0x00E,0x008
};

		//0143416769A7C93A
		byte[] _Conv2 = {
  0x000,0x001,0x004,0x003,0x004,0x001,0x006,0x007, 0x006,0x009,0x00A,0x007,0x00C,0x009,0x003,0x00A
};

		void Init()
		{
			//init Board
			for (byte j = 0; j < 20; j++)
			{
				for (byte i = 0; i < 10; i++)
				{
					Board[i + j * 10] = 0;
				}
			}
			NextShape();
			NextShape();
			DrawNextShape();
		}

		void timing()
		{
			slowdown();
		}

		void rotate()
		{
			NowShapeRotateIdx = (byte)((NowShapeRotateIdx + 1) % 4);

			byte left = GetLeft(NowShapeNum, NowShapeRotateIdx);
			if (left + PosX > 10)
			{
				byte oldPosX = PosX;
				PosX = (byte)(10 - left);
				if (Any_Touch())
				{
					PosX = oldPosX;
					NowShapeRotateIdx = (byte)((NowShapeRotateIdx + 3) % 4);//-1 rotateback
				}
				return;
			}
			if (Any_Touch())
			{
				NowShapeRotateIdx = (byte)((NowShapeRotateIdx + 3) % 4);//-1 rotateback
			}
		}

		void Move(byte n)//+1 -1
		{
			PosX += n;
			if (Any_Touch())
			{
				PosX -= n;
			}
		}

		void slowdown()
		{
			if (Touch())
			{
				TouchDo();
			}
			PosY--;
		}

		void down()
		{
			while (!Touch())
			{
				PosY--;
			}
			TouchDo();
		}

		bool Touch()
		{
			for (byte i = 0; i < 4; i++)
			{
				byte bottom = GetBottom(NowShapeNum, NowShapeRotateIdx, i);
				if (bottom != 0)
				{
					if (bottom == PosY + 1)
					{
						return true;
					}
					if (Board[PosX + i + (PosY - bottom) * 10] != 0)
					{
						return true;
					}
				}
			}
			return false;
		}


		bool Any_Touch()
		{
			if (PosX < 0)
			{
				return true;
			}
			byte left = GetLeft(NowShapeNum, NowShapeRotateIdx);
			if (left + PosX > 10)
			{
				return true;
			}
			for (byte i = 0; i < 4; i++)
			{
				for (byte j = 0; j < 4; j++)
				{
					byte x = (byte)(PosX + i);
					byte y = (byte)(PosY - j);
					byte bottom = GetBottom(NowShapeNum, NowShapeRotateIdx, i);
					if (PosY - bottom < -1)
					{
						return true;
					}
					byte block = GetBlock(NowShapeNum, NowShapeRotateIdx, i, j);
					if (x >= 0 && y >= 0 && x < 10 && y < 20 && block != 0 && Board[x + y * 10] != 0)
					{
						return true;
					}
				}
			}
			return false;
		}


		void TouchDo()
		{
			//add to board
			for (byte i = 0; i < 4; i++)
			{
				for (byte j = 0; j < 4; j++)
				{
					byte x = (byte)(PosX + i);
					byte y = (byte)(PosY - j);
					byte block = GetBlock(NowShapeNum, NowShapeRotateIdx, i, j);
					if (block != 0)
					{
						Board[x + y * 10] = block;
					}
				}
			}

			//clear line
			for (byte j = 19; j != 255; j--)
			{
				byte line = 1;
				for (byte i = 0; i < 10; i++)
				{
					if (Board[i + j * 10] == 0)
					{
						line = 0;
						break;
					}
				}
				if (line == 1)
				{
					for (byte k = 0; k < 10; k++)
					{
						if (j - 1 >= 0)
						{
							Board[k + (j - 1) * 10] = _Conv1[Board[k + (j - 1) * 10]];
						}
						if (j + 1 < 20)
						{
							Board[k + (j + 1) * 10] = _Conv2[Board[k + (j + 1) * 10]];
						}
						Board[k + j * 10] = 0;
					}
					for (byte k = j; k < 19; k++)
					{
						for (byte l = 0; l < 10; l++)
						{
							Board[l + k * 10] = Board[l + (k + 1) * 10];
						}
					}
				}
			}

			//is gameover
			for (byte i = 0; i < 10; i++)
			{
				if (Board[i + 180] != 0)
				{
					Clear();
					break;
				}
			}

			NextShape();
			DrawNextShape();
		}


		void NextShape()
		{
			Random rnd = new Random();
			NowShapeNum = NextShapeNum;
			NextShapeNum = (byte)rnd.Next(0, 6);
			NowShapeRotateIdx = 0;
			PosY = 19;
			PosX = 3;
		}


		void Clear()
		{
			for (byte i = 0; i < 10; i++)
			{
				for (byte j = 0; j < 20; j++)
				{
					Board[i + j * 10] = 0;
				}
			}
		}

		byte GetBlock(byte num, byte idx, byte i, byte j)
		{
			byte ret;
			if ((j & 1) == 1)
			{
				ret = (byte)((_Block[num * 32 + idx * 8 + i * 2 + (j >> 1)] >> 4) & 0x0F);
			}
			else
			{
				ret = (byte)(_Block[num * 32 + idx * 8 + i * 2 + (j >> 1)] & 0x0F);
			}
			return ret;
		}

		byte GetBottom(byte num, byte idx, byte j)
		{
			byte ret;
			if ((j & 1) == 1)
			{
				ret = (byte)((_Bottom[num * 8 + idx * 2 + (j >> 1)] >> 4) & 0x0F);
			}
			else
			{
				ret = (byte)(_Bottom[num * 8 + idx * 2 + (j >> 1)] & 0x0F);
			}
			return ret;
		}

		byte GetLeft(byte num, byte idx)
		{
			byte ret;
			if ((idx & 1) == 1)
			{
				ret = (byte)((_Left[num * 2 + (idx >> 1)] >> 4) & 0x0F);
			}
			else
			{
				ret = (byte)(_Left[num * 2 + (idx >> 1)] & 0x0F);
			}
			return ret;
		}


		void DrawNextShape()
		{
			for (byte i = 0; i < 4; i++)
			{
				for (byte j = 0; j < 4; j++)
				{
					DrawBlock((byte)(20 + j * 3), (byte)(34 + i * 3), GetBlock(NextShapeNum, 0, i, j));
					//ImgNext[j][i].src = GetBlock(NextShapeNum,0,i,j) + ".bmp";//ImgNext[j][i].src = NextShapeArray[0].dot[i][j] + ".bmp";
				}
			}
			refresh();
		}

		void DrawBoard()
		{
			for (byte i = 0; i < 10; i++)
			{
				for (byte j = 0; j < 20; j++)
				{
					DrawBlock((byte)(10 + j * 3), (byte)(2 + i * 3), Board[i + (19 - j) * 10]);
					//ImgBoard[j][i].src = Board[i+(19-j)*10]+".bmp";
				}
			}
			for (byte i = 0; i < 4; i++)
			{
				for (byte j = 0; j < 4; j++)
				{
					byte x = (byte)(i + PosX);
					byte y = (byte)(j + 19 - PosY);
					byte block = GetBlock(NowShapeNum, NowShapeRotateIdx, i, j);
					if (x < 10 && y < 20 && block != 0)
					{
						DrawBlock((byte)(10 + y * 3), (byte)(2 + x * 3), block);
						//ImgBoard[y][x].src = block + ".bmp";;//ImgBoard[y][x].src = NowShape.dot[i][j]+".bmp";
					}
				}
			}
			refresh();
		}

		void refresh()
		{
			Bitmap bmp = new Bitmap(48, 84);

			for (int i = 0; i < 84; i++)
			{
				for (int j = 0; j < 48; j++)
				{
					int idx = i + (j >> 3) * 84;
					int shift = j & 7;
					int c = ((buff[idx] & (1 << shift)) == 0) ? 255 : 0;
					bmp.SetPixel((47-j), i, Color.FromArgb(255, c, c, c));

				}
			}
			pictureBox1.Image = bmp;
		}



		//x:84
		//y:48
		void DrawBlock(byte x, byte y, byte num)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					byte bit = (byte)(blockData[num] & conv[i + j * 3]);
					setPixel(x + (3 - j), y + (3 - i), bit);
				}
			}
		}

		//getpixel x,y (x+y*3) -> 0,1,2,3,8,4,5,6,7
		byte[] conv = { 1, 2, 4, 8, 0, 16, 32, 64, 128 };
		byte[] blockData ={
			0x000,//    0000 0000	00
			//0 000
			//  000
			//  000
			0x0FF,//    1111 1111	FF
			//1 111
			//  101
			//  111
			0x0BD,//    1011 1101	BD
			//2 101
			//  101
			//  101
			0x0E7,//    1110 0111	E7
			//3 111
			//  000
			//  111
			0x0BF,//    1011 1111	BF
			//4 101
			//  101
			//  111
			0x0FD,//    1111 1101	FD
			//5 111
			//  101
			//  101
			0x0EF,//    1110 1111	EF
			//6 111
			//  001
			//  111
			0x0F7,//    1111 0111	F7
			//7 111
			//  100
			//  111
			0x0ED,//    1110 1101	ED
			//8 111
			//  001
			//  101
			0x0B7,//    1011 0111	B7
			//9 101
			//  100
			//  111
			0x0AF,//    1010 1111	AF
			//10101
			//  001
			//  111
			0x0F5,//    1111 0101	F5
			//11111
			//  100
			//  101
			0x0A7,//    1010 0111	A7
			//12101
			//  000
			//  111
			0x0B5,//    1011 0101	B5
			//13101
			//  100
			//  101
			0x0E5,//    1110 0101	E5
			//14111
			//  000
			//  101
			0x0AD//    1010 1101	AD
			//15101
			//  001
			//  101
		};



		private void Form1_Load(object sender, EventArgs e)
		{
			Init();
			timer1.Enabled = true;
			for (int i = 0; i < 84; i++)
			{
				for (int j = 0; j < 48; j++)
				{
					setPixel(i, j, 1);
				}
			}
		}


		void Form1_KeyDown(object sender, System.Windows.Forms.KeyEventArgs e)
		{
			if (e.KeyCode == Keys.Left)
			{
				Move((byte)255);
			}
			if (e.KeyCode == Keys.Right)
			{
				Move((byte)1);
			}
			if (e.KeyCode == Keys.Down)
			{
				down();
			}
			if (e.KeyCode == Keys.Space)
			{
				rotate();
			}
			DrawBoard();
		}

		private void timer1_Tick(object sender, EventArgs e)
		{
			timing();
			//DrawBoard();
		}






	}
}