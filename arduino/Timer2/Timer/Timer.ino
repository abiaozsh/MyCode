volatile int aa;
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
#define dwP13H PORTB |=  _BV(5);
#define dwP13L PORTB &= ~_BV(5);

#define drA3 (PINC & _BV(3))

#define currTick ((TIFR1 & _BV(TOV1))?0x0FFFF:TCNT1)

byte flg=0;
void setup() {
  sei();
  Serial.begin(9600);
  cli();
  
  init();
  while(true)
  {
    settime();
  }

}

void settime()
{
  //reset
  TCNT1 = 0;
  TIFR1 &= !_BV(TOV1);

  while(drA3)
  {
  }
  int v = currTick;
  sei();
  Serial.println(v);
  cli();
  
}

void loop() {
}


void init()
{
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
}







