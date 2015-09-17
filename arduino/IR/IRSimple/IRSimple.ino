#define IRPIN PIND & _BV(3)

uint8_t data[32];

void Setup()
{
  Serial.Begin(9600);
}

void Loop()
{
  while(IRPIN);//�ȴ��͵�ƽ �½���
  //9ms�͵�ƽ
  while(!IRPIN);//�ȴ��ߵ�ƽ ������
  //4.5ms�ߵ�ƽ
  for (int j=0;j<4;j++)         //�ռ���������
  { 
    for (int k=0;k<8;k++)        //ÿ��������8λ
    {
      while(IRPIN);//�ȴ��͵�ƽ �½���
      //0.56ms�͵�ƽ
      while(!IRPIN);//�ȴ��ߵ�ƽ ������
      //?ms�ߵ�ƽ
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
  
  for (int j=0;j<4;j++)         //�ռ���������
  { 
    for (int k=0;k<8;k++)        //ÿ��������8λ
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
