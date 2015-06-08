volatile int aa;
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

void setup() {
  sei();
  Serial.begin(9600);
  cli();
  while(true)
  {
    settime();
  }

}

unsigned int vals[16];
int cnt = 0;
void settime()
{
  for(int i=0;i<16;i++)
  {
    vals[i]=0;
  }


  cnt = 0;
  // COM1A1 COM1A0 COM1B1 COM1B0 – – WGM11 WGM10
  TCCR1A = 0;
  //ICNC1 ICES1 – WGM13 WGM12 CS12 CS11 CS10
  //0 0 1 clkI/O/1 (No prescaling)
  //0 1 0 clkI/O/8 (From prescaler)
  //0 1 1 clkI/O/64 (From prescaler)
  //1 0 0 clkI/O/256 (From prescaler)
  //1 0 1 clkI/O/1024 (From prescaler)

  TCCR1B = 0;
  //sbi(TCCR1B,CS10); // 1/11
  sbi(TCCR1B,CS11);// 1/8

  //FOC1A FOC1B – – – – – –
  TCCR1C = 0;
  //– – ICIE1 – – OCIE1B OCIE1A TOIE1
  TIMSK1 = 0;
  //reset
  TCNT1 = 0;
  TIFR1 &= !TOV1;
  while(true)
  {
    byte TCoverflow = TIFR1 & _BV(TOV1);
    int v = TCNT1;
    for(int dd=0;dd<5000;dd++)
    {
      aa++;
    }
    if(cnt<16)
    {
      vals[cnt] = v;
      cnt++;
    }
    else
    {
      break;
    }
    if(TCoverflow)
    {
      break;
    }
  }
  sei();
  Serial.print("cnt:");
  Serial.println(cnt);
  for(int i=0;i<16;i++)
  {
    Serial.print(i);
    Serial.print(":");
    Serial.println(vals[i]);
  }
  Serial.println("");
  Serial.println("");
  cli();
  for(long i=0;i<1000000;i++)
  {
    aa++;
  }
}

void loop() {
}






