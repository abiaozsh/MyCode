void setup() {
  Serial.begin(9600);
}

void loop() {
  int readdata=0;
  while(readdata==0)
  {
    Serial.println("input:");
    while (Serial.available() <= 0);
    readdata = Serial.parseInt(); 
  }

  unsigned long CurrentTime = micros();

  float r = getR(readdata);//digitalRead(A0)
  float t=(1.0/(log(r/9700.0)/3800.0 + 1.0/(273.15+25.0))) - 273.15;
  unsigned long time =  micros() - CurrentTime;

  Serial.print(readdata);
  Serial.print("\t");
  Serial.print(r);
  Serial.print("\t");
  Serial.print(t);
  Serial.print("\t");
  Serial.println(time);
  delay(100);
}

float getT(float r)
{
  float t=(1.0/(log(r/9700.0)/3800.0 + 1.0/(273.15+25.0))) - 273.15;
  return t;
}

float getR(int aread)
{
  //float v2 = 5.0f * aread / 1024.0f;
  float v2 = 35.0f - (5.0f * aread / 1024.0f);
  float v1 = 35.0f;
  float r1 = 455.0f;
  //  r2=(V2/V1)*5000/(1-(V2/V1));
  float r = (v2 / v1) * r1 / (1 - (v2 / v1));
  return r;
}


