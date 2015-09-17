#define IRPIN PIND & _BV(3)

uint16_t data[32];

void setup()
{
  TCCR0A = 0;
  TCCR01 = 1;
  Serial.begin(9600);
}

void loop()
{
  while(IRPIN);//等待低电平 下降沿
  //9ms低电平
  while(!IRPIN);//等待高电平 上升沿
  //4.5ms高电平
  for (int j=0;j<4;j++)         //收集四组数据
  { 
    for (int k=0;k<8;k++)        //每组数据有8位
    {
      while(IRPIN);//等待低电平 下降沿
      //0.56ms低电平
      while(!IRPIN);//等待高电平 上升沿
      //?ms高电平
      TCNT1=0;
      while(IRPIN)
      {
        if(TCNT1>65000){break;}
      }
      data[j*8+k]=TCNT1;
    }
  }
  
  for (int j=0;j<4;j++)         //收集四组数据
  { 
    for (int k=0;k<8;k++)        //每组数据有8位
    {
      Serial.print(data[j*8+k]);
      Serial.print(",");
    }
    Serial.println();
  }
}

