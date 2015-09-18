#define IRPIN (PIND & _BV(2))
#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)
/*
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
  Serial.println("fdsa");
}
void RAWTimeline()
{/*
  uint16_t data[512];
  uint16_t dataidx;
  TCCR1A = 0;
  TCCR1B = 4;//1 0 0 clkI/O/256 (From prescaler)
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
  }*/
}
uint8_t GetIR()
{
  uint32_t data = 0;
  uint8_t dataidx = 0;
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
  while((!(TIFR0 & _BV(TOV0))) && (dataidx<32))
  {
    if(OLDflg != IRPIN)
    {
      if(IRPIN)
      {
      }
      else
      {
        //35~105 70
        data<<=1;
        if(TCNT0>70)
        {
          data|=1;
        }
        dataidx++;
      }
      TCNT0 = 0;
      OLDflg = IRPIN;
    }
  }
  DDRB&=~_BV(5);
  PORTB&=~_BV(5);
  if(data==0)
  {
    return 0;
  }
  if(((data>>24)&0xFF)!=0)
  {
    return 0xFF;
  }
  if(((data>>16)&0xFF)!=0xFF)
  {
    return 0xFF;
  }
  if(((data>>8)&0xFF)!=((~data)&0xFF))
  {
    return 0xFF;
  }
  return data&0xFF;
}
void loop()
{
    Serial.println(GetIR(),HEX);
}

