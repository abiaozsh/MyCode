#define CLKON PORTD |= _BV(3)
#define CLKOFF PORTD &= ~_BV(3)
#define DATON PORTD |= _BV(2)
#define DATOFF PORTD &= ~_BV(2)

#define _DELAY_ delayMicroseconds(100);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.setTimeout(1000000);
  DDRD = _BV(2) | _BV(3);//2 clock 3 data
  DDRB |= _BV(5);
  CLKOFF;
  DATOFF;//startbit
  _DELAY_;
  for(int i=0;i<10;i++)
  {
    CLKON;//rise send data
    _DELAY_;
    CLKOFF;//fall
    _DELAY_;
  }
  send(0);
}

void loop() {
  int v1 = Serial.read();
  if(v1!=-1)
  {
    PINB |= _BV(5);
    uint8_t val = (uint8_t)v1;
    send(val);
  }
}
void send(uint8_t val)
{

  DATON;//startbit
  _DELAY_;
  CLKON;//rise send data
  _DELAY_;
  CLKOFF;//fall
  _DELAY_;

  for(uint8_t i=0;i<8;i++)
  {
    if(val&0x80)
    {
      DATON;//1
    }
    else
    {
      DATOFF;//0
    }
    val<<=1;
    _DELAY_;
    CLKON;//rise send data
    _DELAY_;
    CLKOFF;//fall
    _DELAY_;
  }
}

