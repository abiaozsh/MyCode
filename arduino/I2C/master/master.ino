#include <Wire.h>
void setup()
{
  Wire.begin(); // join i2c bus (address optional for master)
  pinMode(A0, INPUT);
  pinMode(9, INPUT);
}

byte x = 0;
int val = 0;
void loop()
{
  Wire.beginTransmission(4); // transmit to device #4
  Wire.write(x);              // sends one byte  
  Wire.endTransmission();    // stop transmitting
  x++;
  
  while(val == digitalRead(A0))
  {
  }
  val == digitalRead(A0);
  digitalWrite(9, val);
}
