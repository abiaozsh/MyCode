#define LEDAOUT DDRB |= _BV(0);
#define LEDBOUT DDRD |= _BV(5);
#define LEDAON  PORTB &= ~_BV(0);
#define LEDAOFF PORTB |=  _BV(0);
#define LEDBON  PORTD &= ~_BV(5);
#define LEDBOFF PORTD |=  _BV(5);
//PB0
//PD5
void setup() {
  LEDAOUT;
  LEDBOUT;
}
volatile int temp;
void loop() {
  LEDAON;
  for(int i=0;i<3;i++)
  {
    temp++;
  }
  //delay(300);
  LEDAOFF;
  delay(300);
  LEDBON;
  for(int i=0;i<3;i++)
  {
    temp++;
  }
  //delay(300);
  LEDBOFF;
  delay(300);
}



