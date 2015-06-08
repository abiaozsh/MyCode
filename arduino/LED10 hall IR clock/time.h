byte days = 0;
byte hour = 0;
byte miniute = 0;
byte second = 0;
int tick = 0;//15625 tick/s
byte dimeSeconds=0;

uint16_t tick3 = 0;

byte figure1;
byte figure2;
byte figure3;
byte figure4;
byte figure5;
byte figure6;

void IncTimer(int inc)
{
  tick+=inc;
  if(tick>=15625)
  {
    tick -= 15625;
    second++;
    if(second==60)
    {
      second = 0;
      miniute++;
      if(miniute==60)
      {
        miniute = 0;
        hour++;
        if(hour==24)
        {
          hour=0;
          days++;
        }
      }
    }
  }
  unsigned long temp = tick;
  dimeSeconds = (byte)((temp*10737)>>24);//0~9
  
}


