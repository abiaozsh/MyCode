#include <SD.h>

File myFile;

void setup()
{
  Serial.begin(9600);

  pinMode(13, OUTPUT);
}

void loop()
{

  while (!SD.begin()) {
    delay(10);
    digitalWrite(13,HIGH);
    delay(10);
    digitalWrite(13,LOW);
  }

  //while(true)
  {
    // re-open the file for reading:
    myFile = SD.openSimple("TEST1.TXT", O_READ, 0);//openSimple
    if (myFile) {
      int v = myFile.read();
      Serial.println("v");
      Serial.println(v);
      if(v!=97)//'a'
      {
        digitalWrite(13,HIGH);
        return;
      }
      myFile.close();
    } 
    else {
      _blink(100,1000);
    }
  }
  

  {
    myFile = SD.openSimple("TEST2.TXT", O_WRITE, 1);
    if (myFile) {
      myFile.println("a");
      myFile.close();
    } 
    else {
      _blink(1000,100);
    }
  }
  
  {
    myFile = SD.openSimple("TEST3.TXT", O_WRITE | O_TRUNC, 0);
    if (myFile) {
      myFile.println("b");
      myFile.close();
    } 
    else {
      _blink(1000,1000);
    }
  }
  
  while(true)
  {
  }
}

void _blink(int aa,int bb)
{
  while(true)
  {
    delay(aa);
    digitalWrite(13,HIGH);
    delay(bb);
    digitalWrite(13,LOW);
  }
}




