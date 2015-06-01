//getpixel x,y (x+y*3) -> 0,1,2,3,8,4,5,6,7
PROGMEM prog_uint8_t conv[] = { 
  1, 2, 4, 8, 0, 16, 32, 64, 128 };
PROGMEM prog_uint8_t blockData[] ={
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


void InitImg()
{
  for (byte i = 0; i < 84; i++)
  {
    for (byte j = 0; j < 48; j++)
    {
      setPixel(i, j, 1);
    }
  }
}


//x:84
//y:48
void setPixel(byte x, byte y, byte val)
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

//x:84
//y:48
void DrawBlock(byte x, byte y, byte num)
{
  for (byte i = 0; i < 3; i++)
  {
    for (byte j = 0; j < 3; j++)
    {
      byte bit = (byte)(pgm_read_byte_near(blockData + num) & pgm_read_byte_near(conv + i + j * 3));
      setPixel(x + (3 - j), y + (3 - i), bit);
    }
  }
}

void DrawNextShape()
{
  for(byte i=0;i<4;i++)
  {
    for(byte j=0;j<4;j++)
    {
      DrawBlock((byte)(20 + j * 3), (byte)(34 + i * 3), GetBlock(NextShapeNum, 0, i, j));
    }
  }
}

void DrawBoard()
{
  for(byte i=0;i<10;i++)
  {
    for(byte j=0;j<20;j++)
    {
      DrawBlock((byte)(10 + j * 3), (byte)(2 + i * 3), Board[i + (19 - j) * 10]);
    }
  }
  for(byte i=0;i<4;i++)
  {
    for(byte j=0;j<4;j++)
    {
      byte x = i+PosX;
      byte y = j+19-PosY;
      byte block = GetBlock(NowShapeNum,NowShapeRotateIdx,i,j);
      if(x<10&&y<20&&block!=0)
      {
        DrawBlock((byte)(10 + y * 3), (byte)(2 + x * 3), block);
      }
    }
  }
}

