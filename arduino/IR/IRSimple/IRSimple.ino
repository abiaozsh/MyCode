#define IRPIN PIND & _BV(3)

uint8_t data[32];

void Setup()
{
  Serial.Begin(9600);
}

void Loop()
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
      uint8_t cnt = 0;
      while(IRPIN)
      {
        delay14();
        cnt++;
        if(cnt>100){break;}
      }
      data[j*8+k]=cnt;
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

//TCCR0B = 3; //0 1 1 clkI/O/64 (From prescaler) 16Mhz/64
//0.0000000625 s/tick(base)
//0.000004 s/tick(*64)
//0.004 ms/tick(*64)
//250tick/ms
void delay14()//0.004*35 = 0.14ms
{
  TCNT0=0;
  while(TCNT0<35);
}
