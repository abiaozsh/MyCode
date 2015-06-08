
void setup() {                
  pinMode(9, OUTPUT);     
}

void loop() {
  float t = getT(getR(digitalRead(A0)));
  Read(t,2);

  /*
  Read(D0,len_D0);    delay(500);
   Read(D1,len_D1);    delay(500);
   Read(D2,len_D2);    delay(500);
   Read(D3,len_D3);    delay(500);
   Read(D4,len_D4);    delay(500);
   Read(D5,len_D5);    delay(500);
   Read(D6,len_D6);    delay(500);
   Read(D7,len_D7);    delay(500);
   Read(D8,len_D8);    delay(500);
   Read(D9,len_D9);    delay(500);
   Read(W1yao,len_W1yao);    delay(500);
   Read(W2shi,len_W2shi);    delay(500);
   Read(W3bai,len_W3bai);    delay(500);
   //Read(W4qian,len_W4qian);    delay(500);
   //Read(W5wan,len_W5wan);    delay(500);
   Read(W6dian,len_W6dian);    delay(500);
   Read(W7fu,len_W7fu);    delay(500);
   Read(W8fen,len_W8fen);    delay(500);
   Read(W9miao,len_W9miao);    delay(500);
   */
}
void Read(int d)
{
  switch (d)
  {
  case 0: 
    Read(D0,len_D0); 
    break;
  case 1: 
    Read(W1yao,len_W1yao); 
    break;
  case 2: 
    Read(D2,len_D2); 
    break;
  case 3: 
    Read(D3,len_D3); 
    break;
  case 4: 
    Read(D4,len_D4); 
    break;
  case 5: 
    Read(D5,len_D5); 
    break;
  case 6: 
    Read(D6,len_D6); 
    break;
  case 7: 
    Read(D7,len_D7); 
    break;
  case 8: 
    Read(D8,len_D8); 
    break;
  case 9: 
    Read(D9,len_D9); 
    break;
  }
}
void Read(float number, int digits)
{
  if (number < 0.0)
  {
    Read(W7fu,len_W7fu);
    number = -number;
  }

  // Extract the integer part of the number and print it
  int int_part = (int)number;
  double remainder = number - (double)int_part;
  {
    int d10000 = 0;
    int d1000 = 0;
    int d100 = 0;
    int d10 = 0;
    int d1 = 0;

    d1 = int_part % 10;
    int_part /= 10;
    d10 = int_part % 10;
    int_part /= 10;
    d100 = int_part % 10;
    int_part /= 10;
    d1000 = int_part % 10;
    int_part /= 10;
    d10000 = int_part % 10;

    byte d10flg = 0;
    if (d10000 > 0)
    {
      Read(d10000);
      Read(W5wan,len_W5wan);
      if (d1000 == 0)
      {
        Read(D0,len_D0);
      }
      d10flg = 1;
    }
    if (d1000 > 0)
    {
      Read(d1000);
      Read(W4qian,len_W4qian);
      if (d100 == 0)
      {
        Read(D0,len_D0);
      }
      d10flg = 1;
    }
    if (d100 > 0)
    {
      Read(d100);
      Read(W3bai,len_W3bai);
      d10flg = 1;
      if (d10 == 0)
      {
        Read(D0,len_D0);
      }
    }
    if (d10 > 0)
    {
      if (d10 > 1 || d10flg==1)
      {
        Read(d10);
      }
      Read(W2shi,len_W2shi);
    }
    if (d1 > 0)
    {
      Read(d1);
    }
  }

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
  {
    Read(W6dian,len_W6dian);
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = (int)(remainder);
    Read(toPrint);
    remainder -= toPrint;
  }
}



void Read(prog_uint8_t* point,int len)
{
  int flg=LOW;
  for(int i=0;i<len;i++)
  {
    digitalWrite(9, flg);
    flg=!flg;
    for(byte j=0;j<pgm_read_byte_near(point + i);j++)
    {
      delayMicroseconds(21);//1000000/44100Hz = 22.675736961451247165532879818594
    }
  }
  digitalWrite(9, LOW);
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




