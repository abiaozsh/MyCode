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
  Serial.begin(115200); 
  pinMode(10, OUTPUT);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}
}

void loop() {
  TCCR0A = 0;
  TCCR0B = 5;//clkI/O/1024 (From prescaler) 30.517578125 times / s
  TIMSK0 = 0;
  TCNT0 = 0;
  uint8_t a = 0;
  for(;;)
  {
    while(!(TIFR0&_BV(TOV0)));
    TIFR0 |= _BV(TOV0);
    Serial.print("a");
    if(a){
      a=0;
      LEDAON;
      LEDBOFF;
      digitalWrite(10, HIGH);
    }else{
      a=1;
      LEDAOFF;
      LEDBON;
      digitalWrite(10, LOW);
    }
  }
}



