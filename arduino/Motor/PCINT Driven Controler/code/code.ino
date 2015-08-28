void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  DDRD = _BV(2) | _BV(3);//2 clock 3 data

}

void loop() {
  int v1 = Serial.parseInt();
  int v2 = Serial.parseInt();
  if(v1>0)
  {
    uint16_t val = v1;
    if(v2)
    {
      val|=0x8000;
    }

    PORTD |= _BV(3);//startbit
    delay(1);
    PORTD |= _BV(2);//rise send data
    delay(1);
    PORTD &= ~_BV(2);//fall
    delay(1);


    for(uint8_t i=0;i<16;i++)
    {
      if(val&0x8000)
      {
        PORTD |= _BV(3);//1
      }
      else
      {
        PORTD &= ~_BV(3);//0
      }
      val<<=1;
      delay(1);
      PORTD |= _BV(2);//rise send data
      delay(1);
      PORTD &= ~_BV(2);//fall
      delay(1);
    }
  }
}

