
#include <Wire.h>

unsigned long oldtime;
void setup()
{
  Wire.begin(4);                // join i2c bus with address #4
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(9600);           // start serial for output
  oldtime = micros();
}
int count=0;

void loop()
{
  int val = 0;
  for(int i=0;i<1000;i++)
  {
    val++;
  }
  unsigned long time = micros();
  unsigned long diff = time - oldtime;
  if(diff > 110 || diff < 100)
  {
    Serial.println(diff);
    if(count++>100)
    {
      while(true){;};
    }
  }
  oldtime = time;
}

byte x = 0;
void receiveEvent(int howMany)
{
  while(1 < Wire.available())
  {
    x = Wire.read();
  }
}
