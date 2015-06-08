const int ledPin = 13;      // the pin that the LED is attached to

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

byte state=0;

void loop() {
  byte brightness;
  // check if data has been sent from the computer:
  if (Serial.available()) {
    byte d = Serial.read();
    if(d==0)
    {
      // set the brightness of the LED:
      digitalWrite(ledPin, HIGH);
    }
    if(d==1)
    {
      // set the brightness of the LED:
      digitalWrite(ledPin, LOW);
    }
  }
}





