//D,C,E,B,F
//A0 - PF7


void setup() {
  InitIR();
  InitTimer();

  cli();
  initArray();
  rstPin();
  Timer0Init();
  Timer1Init();
  Timer3Init();
  initDDR();


  loop();
}

byte data_line[30];

unsigned int lastTick = 0;//15625 tick/s
unsigned int tickCount = 0;//15625 tick/s

void loop() {
  for(;;)
  {
    while((PINF&_BV(7))!=0){
    }
    unsigned int buffTick = TCNT3;
    unsigned int diffTick = buffTick - lastTick;
    lastTick = buffTick;
    IncTimer(diffTick);

    ShowROM(figure1*7,figure1*7+7,data_num);
    ShowROM(figure2*7,figure2*7+7,data_num);
    ShowROM(70,77,data_num);
    ShowROM(figure3*7,figure3*7+7,data_num);
    ShowROM(figure4*7,figure4*7+7,data_num);
    ShowROM(70,77,data_num);
    ShowROM(figure5*7,figure5*7+7,data_num);
    ShowROM(figure6*7,figure6*7+7,data_num);

    for(int i=0;i<10;i++)
    {
      if(i>dimeSeconds)
      {
        data_line[i*3] = 0;
        data_line[i*3+1] = 0;
        data_line[i*3+2] = 0;
      }
      else
      {
        data_line[i*3] = 1;
        data_line[i*3+1] = 1;
        data_line[i*3+2] = 1;
      }
    }

    ShowRAM(0,1,data_line);

    /*
    if(idx>= 0&&idx<10)
     {
     ShowROM(cnt_p1,data_p1);
     }
     if(idx>=10&&idx<20)
     {
     ShowROM(cnt_p3,data_p3);
     }
     if(idx>=20&&idx<30)
     {
     ShowROM(cnt_p2,data_p2);
     }
     if(idx>=30&&idx<40)
     {
     ShowROM(cnt_p3,data_p3);
     }*/

    //ShowRAM(1,data_line);

  }
}




