#define IRPIN (PINB & _BV(4))
#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)
/*
正确
45 46 47
44 40 43
07 15 09
16 19 0D
0C 18 5E
08 1C 5A
42 52 4A

高低位反了



5D 9D 1D
DD FD 3D
1F 57 6F
97 67 4F
CF E7 85
EF C7 A5
BD B5 AD
*/
void setup()
{
  Serial.begin(9600);
}
void RAWTimeline()
{
  uint16_t data[512];
  uint16_t dataidx;
  TCCR1A = 0;
  TCCR1B = 4;//1 0 0 clkI/O/256 (From prescaler)
  TIMSK1 = 0;
  while(IRPIN);//等待低电平 下降沿
  uint8_t OLDflg = IRPIN;
  dataidx = 0;
  TCNT1 = 0;
  TIFR1 |= _BV(TOV1);
  while(!(TIFR1 & _BV(TOV1)))
  {
    if(OLDflg != IRPIN)
    {
      data[dataidx++] = TCNT1;
      OLDflg = IRPIN;
    }
  }
  
  Serial.println(dataidx);
  for(uint16_t i=1;i<dataidx;i++)
  {
    uint16_t dif = data[i] - data[i-1];
    Serial.print(i);
    Serial.print("\t");
    Serial.println(dif);
  }
}
uint8_t GetIR()
{
  uint8_t data = 0;
  uint8_t datahi = 0;
  uint8_t dataidx = 0;
  uint8_t byteidx = 0;
  TCCR0A = 0;
  TCCR0B = 4;//1 0 0 clkI/O/256 (From prescaler)
  while(IRPIN);//等待低电平 下降沿
  DDRB|=_BV(5);
  PORTB|=_BV(5);
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
        if(TCNT0<70)
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
  DDRB&=~_BV(5);
  PORTB&=~_BV(5);
  if(data!=datahi)
  {
    return 0xFF;
  }
  return data;
}
void loop()
{
    //RAWTimeline();
    Serial.println(GetIR(),HEX);
}

