
uint8_t GetIR()
{
  uint8_t data = 0;
  uint8_t datahi = 0;
  uint8_t dataidx = 0;
  uint8_t byteidx = 0;
  TCCR0A = 0;
  TCCR0B = 4;//0 1 1 clkI/O/64 (From prescaler)
  while(IRPIN);//等待低电平 下降沿
  while(!IRPIN);
  while(IRPIN);
  uint8_t OLDflg = IRPIN;
  TCNT0 = 0;
  TIMSK0 = 0;
  TIFR0 |= _BV(TOV0);
  while((!(TIFR0 & _BV(TOV0))) && (byteidx<4))
  {
    if(OLDflg != IRPIN)
    {
      if(IRPIN)
      {
      }
      else
      {
        //35~105 70
        data>>=1;
        if(TCNT0<35)
        {
          data|=0x80;
        }
        dataidx++;
        if(dataidx==8)
        {
          dataidx=0;
          switch(byteidx)
          {
            case 0:
              if(data!=0xFF)return 0xFF;
              break;
            case 1:
              if(data!=0x00)return 0xFF;
              break;
            case 2:
              datahi = ~data;
              break;
          }
          byteidx++;
        }
      }
      TCNT0 = 0;
      OLDflg = IRPIN;
    }
  }
  if(data!=datahi)
  {
    return 0xFF;
  }
  return data;
}
/*
void RAWTimeline()
{
  uint16_t data[32];
  uint16_t dataidx;
  TCCR1A = 0;
  TCCR1B = 4;//1 0 0 clkI/O/256 (From prescaler)
  TIMSK1 = 0;
  while(IRPIN);//等待低电平 下降沿
  uint8_t OLDflg = IRPIN;
  dataidx = 0;
  TCNT1 = 0;
  TIFR1 |= _BV(TOV1);
  while(!(TIFR1 & _BV(TOV1)) && dataidx<32)
  {
    if(OLDflg != IRPIN)
    {
      data[dataidx++] = TCNT1;
      OLDflg = IRPIN;
    }
  }
  
  SendInt(dataidx);
  for(uint16_t i=1;i<dataidx;i++)
  {
    uint16_t dif = data[i] - data[i-1];
    SendInt(i);
    SerialSend('\t');
    SendInt(dif);
	SerialSend('\r');
	SerialSend('\n');
  }
}
*/