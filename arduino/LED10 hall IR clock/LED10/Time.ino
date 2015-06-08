
void IncTimer(unsigned int inc)
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
  
  figure1 = hour / 10;
  figure2 = hour % 10;
  
  figure3 = miniute / 10;
  figure4 = miniute % 10;

  figure5 = second / 10;
  figure6 = second % 10;
  
  dimeSeconds = (tick*10737)>>24;//0~9
  
}



