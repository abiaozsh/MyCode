
LCD5110::LCD5110(int SCK, int MOSI, int DC, int RST, int CS)
{ 
  P_SCK	= portOutputRegister(digitalPinToPort(SCK));
  B_SCK	= digitalPinToBitMask(SCK);
  P_MOSI	= portOutputRegister(digitalPinToPort(MOSI));
  B_MOSI	= digitalPinToBitMask(MOSI);
  P_DC	= portOutputRegister(digitalPinToPort(DC));
  B_DC	= digitalPinToBitMask(DC);
  P_RST	= portOutputRegister(digitalPinToPort(RST));
  B_RST	= digitalPinToBitMask(RST);
  P_CS	= portOutputRegister(digitalPinToPort(CS));
  B_CS	= digitalPinToBitMask(CS);
  pinMode(SCK,OUTPUT);
  pinMode(MOSI,OUTPUT);
  pinMode(DC,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(CS,OUTPUT);
  SCK_Pin=SCK;
  RST_Pin=RST;
}

void LCD5110::_LCD_Write(unsigned char data, unsigned char mode)
{   
  cbi(P_CS, B_CS);

  if (mode==LCD_COMMAND)
    cbi(P_DC, B_DC);
  else
    sbi(P_DC, B_DC);

  for (unsigned char c=0; c<8; c++)
  {
    if (data & 0x80)
      sbi(P_MOSI, B_MOSI);
    else
      cbi(P_MOSI, B_MOSI);
    data = data<<1;
    pulseClock;
  }

  sbi(P_CS, B_CS);
}

void LCD5110::InitLCD(int contrast)
{
  if (contrast>0x7F)
    contrast=0x7F;
  if (contrast<0)
    contrast=0;

  resetLCD;

  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
  _LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  for (int c=0; c<504; c++)
    _LCD_Write(0x00, LCD_DATA);
  _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);

  cfont.font=0;
  _sleep=false;
  _contrast=contrast;
}

void LCD5110::setContrast(int contrast)
{
  if (contrast>0x7F)
    contrast=0x7F;
  if (contrast<0)
    contrast=0;
  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _contrast=contrast;
}
/*
void LCD5110::enableSleep()
{
  _sleep = true;
  _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
  _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  for (int b=0; b<504; b++)
    _LCD_Write(0, LCD_DATA);
  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_POWERDOWN, LCD_COMMAND);
}

void LCD5110::disableSleep()
{
  _LCD_Write(PCD8544_FUNCTIONSET | PCD8544_EXTENDEDINSTRUCTION, LCD_COMMAND);
  _LCD_Write(PCD8544_SETVOP | _contrast, LCD_COMMAND);
  _LCD_Write(PCD8544_SETTEMP | LCD_TEMP, LCD_COMMAND);
  _LCD_Write(PCD8544_SETBIAS | LCD_BIAS, LCD_COMMAND);
  _LCD_Write(PCD8544_FUNCTIONSET, LCD_COMMAND);
  _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
  _sleep = false;
}
*/
void LCD5110::clrScr()
{
  if (!_sleep)
  {
    _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
    for (int c=0; c<504; c++)
      _LCD_Write(0x00, LCD_DATA);
  }
}

void LCD5110::invert(bool mode)
{
  if (!_sleep)
  {
    if (mode==true)
      _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYINVERTED, LCD_COMMAND);
    else
      _LCD_Write(PCD8544_DISPLAYCONTROL | PCD8544_DISPLAYNORMAL, LCD_COMMAND);
  }
}

void LCD5110::drawBitmap(int x, int y, bitmapdatatype bitmap, int sx, int sy)
{
  int starty, rows;

  if (!_sleep)
  {
    starty = y / 8;

    if (sy%8==0)
      rows=sy/8;  
    else
      rows=(sy/8)+1;

    for (int cy=0; cy<rows; cy++)
    {
      _LCD_Write(PCD8544_SETYADDR | (starty+cy), LCD_COMMAND);
      _LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);
      for(int cx=0; cx<sx; cx++)
        _LCD_Write(bitmapbyte(cx+(cy*sx)), LCD_DATA);
    }      
    _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  }
}

void LCD5110::drawBitmapBuff(int x, int y, byte *bitmap, int sx, int sy)
{
  int starty, rows;

  if (!_sleep)
  {
    starty = y / 8;

    if (sy%8==0)
      rows=sy/8;  
    else
      rows=(sy/8)+1;

    for (int cy=0; cy<rows; cy++)
    {
      _LCD_Write(PCD8544_SETYADDR | (starty+cy), LCD_COMMAND);
      _LCD_Write(PCD8544_SETXADDR | x, LCD_COMMAND);
      for(int cx=0; cx<sx; cx++)
        _LCD_Write(bitmap[cx+(cy*sx)], LCD_DATA);
    }      
    _LCD_Write(PCD8544_SETYADDR, LCD_COMMAND);
    _LCD_Write(PCD8544_SETXADDR, LCD_COMMAND);
  }
}


