inline void _dly()
{
  IOWR(MYTIMER, 0, 0);
  while(1)
  {
	  int time = IORD(MYTIMER, 0);
	  if(time > 5){
		  break;
	  }
  }
}


void SPI_CHIP_SELECT_HIGH(){
  IOWR(SOFTSPI, SOFTSPI_CS, 0x07);
}
void SPI_CHIP_SELECT_LOW(int chip){
  IOWR(SOFTSPI, SOFTSPI_CS, ~(1<<chip));
}

//------------------------------------------------------------------------------
int spiRec() {
  int data = 0;
  IOWR(SOFTSPI, SOFTSPI_MOSI, 1);
  int i;
  for (i = 0; i < 8; i++) {
    IOWR(SOFTSPI, SOFTSPI_SCK, 1);
    _dly();
    data <<= 1;
    
    if (IORD(SOFTSPI, SOFTSPI_MISO)) data |= 1;

    IOWR(SOFTSPI, SOFTSPI_SCK, 0);
    _dly();
  }
  return data;
}
//------------------------------------------------------------------------------
void spiSend(int data) {
  int i;
  for (i = 0; i < 8; i++) {
    IOWR(SOFTSPI, SOFTSPI_SCK, 0);
    _dly();
    if(data & 0x80)
    {
      IOWR(SOFTSPI, SOFTSPI_MOSI, 1);
    }
    else
    {
      IOWR(SOFTSPI, SOFTSPI_MOSI, 0);
    }
    data <<= 1;
    IOWR(SOFTSPI, SOFTSPI_SCK, 1);
    _dly();
  }
  IOWR(SOFTSPI, SOFTSPI_SCK, 0);
}
