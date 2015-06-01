#include <SoftwareSerial.h>
SoftwareSerial mySerial(0, 1); // RX, TX

void setup()  
{
  mySerial.begin(115200);
  mySerial.write('H');
  mySerial.write('e');
  mySerial.write('l');
  mySerial.write('l');
  mySerial.write('o');
  mySerial.write('\r');
  mySerial.write('\n');
  while(true)
  {
    loop();
  }
}
void loop() // run over and over
{
  uint8_t b = mySerial.read();
  if(b!=0x0FF)
  {
  uint8_t i=0x80;
  while(i)
  {
    if(b&i)
    {
    mySerial.write('1');
    }
    else
    {
    mySerial.write('0');
    }
    i>>=1;
  }
  mySerial.write(b);
  mySerial.write('1');
  mySerial.write('2');
  mySerial.write('3');
  mySerial.write('4');
  mySerial.write('5');
  mySerial.write('6');
  mySerial.write('7');
  mySerial.write('8');
  mySerial.write('9');
  mySerial.write('0');
  mySerial.write('a');
  mySerial.write('b');
  mySerial.write('c');
  mySerial.write('d');
  mySerial.write('e');
  mySerial.write('f');
  mySerial.write('g');
  mySerial.write('h');
  mySerial.write('i');
  mySerial.write('j');
  mySerial.write('k');
  mySerial.write('l');
  mySerial.write('m');
  mySerial.write('n');
  mySerial.write('o');
  mySerial.write('p');
  mySerial.write('q');
  mySerial.write('r');
  mySerial.write('\r');
  mySerial.write('\n');
  //delay(500);
  //if (mySerial.available())
  //  Serial.write(mySerial.read());
  //if (Serial.available())
  //  mySerial.write(Serial.read());
  }
}

