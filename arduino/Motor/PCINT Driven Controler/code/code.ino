#define CLKON PORTD |= _BV(3)
#define CLKOFF PORTD &= ~_BV(3)
#define DATON PORTD |= _BV(2)
#define DATOFF PORTD &= ~_BV(2)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(1000000);
  DDRD = _BV(2) | _BV(3);//2 clock 3 data
  CLKOFF;
  DATOFF;
  DATOFF;//startbit
  delay(1);
  for(int i=0;i<20;i++)
  {
    CLKON;//rise send data
    delay(1);
    CLKOFF;//fall
    delay(1);
  }
  send(0);
}

void loop() {
  int v1 = Serial.parseInt();
  int v2 = Serial.parseInt();
  {
    uint16_t val = v1;
    if(v2)
    {
      val|=0x8000;
    }
    send(val);
  }
}
void send(uint16_t val)
{

  DATON;//startbit
  delay(1);
  CLKON;//rise send data
  delay(1);
  CLKOFF;//fall
  delay(1);

  Serial.println(val);
  for(uint8_t i=0;i<16;i++)
  {
    if(val&0x8000)
    {
      DATON;//1
    }
    else
    {
      DATOFF;//0
    }
    val<<=1;
    delay(1);
    CLKON;//rise send data
    delay(1);
    CLKOFF;//fall
    delay(1);
  }
}

