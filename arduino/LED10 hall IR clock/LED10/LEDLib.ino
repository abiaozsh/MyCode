uint8_t ValueR[10];
volatile uint8_t* PORTSR[10];
uint8_t BITSR[10];

uint8_t ValueG[10];
volatile uint8_t* PORTSG[10];
uint8_t BITSG[10];

uint8_t ValueB[10];
volatile uint8_t* PORTSB[10];
uint8_t BITSB[10];

void ShowROM(int start,int cnt,prog_uint8_t* data)
{
  for(int idx = start;idx<cnt;idx++)
  {
    ResetT1;
    //init
    int temp = idx * 30;
    for(uint8_t i=0;i<10;i++)
    {
      int temp2 = temp + i*3;
      ValueR[i] = pgm_read_byte_near(data+temp2);
      PORTSR[i] = PORTS10[i];
      BITSR[i]  = BITS10[i];
      ValueG[i] = pgm_read_byte_near(data+temp2+1);
      PORTSG[i] = PORTS10[i];
      BITSG[i]  = BITS10[i];
      ValueB[i] = pgm_read_byte_near(data+temp2+2);
      PORTSB[i] = PORTS10[i];
      BITSB[i]  = BITS10[i];
    }
    ShowCore();
  }
}

void ShowRAM(int start,int cnt,byte* data)
{
  for(int idx = start;idx<cnt;idx++)
  {
    ResetT1;
    //init
    int temp = idx * 30;
    for(uint8_t i=0;i<10;i++)
    {
      int temp2 = temp + i*3;
      ValueR[i] = data[temp2];
      PORTSR[i] = PORTS10[i];
      BITSR[i]  = BITS10[i];
      ValueG[i] = data[temp2+1];
      PORTSG[i] = PORTS10[i];
      BITSG[i]  = BITS10[i];
      ValueB[i] = data[temp2+2];
      PORTSB[i] = PORTS10[i];
      BITSB[i]  = BITS10[i];
    }
    ShowCore();
  }
}

void ShowCore()
{
  SortR(0,9);
  SortG(0,9);
  SortB(0,9);
  while(currTick1<0x0150){//TODO
  }

  {
    PORTF = _BV(4);
    //turn on
    for(uint8_t i=0;i<10;i++)  
    {
      if(ValueR[i]!=0)
      {
        *PORTSR[i] &= ~BITSR[i];//low
      }
    }  
    ResetT0;
    for(uint8_t i=0;i<10;i++)
    {
      uint8_t trigger = ValueR[i];
      while(currTick0<trigger)
      {
      }
      *PORTSR[i] |= BITSR[i];
    }
    PORTF = 0;
  }
  {
    PORTF = _BV(5);
    //turn on
    for(uint8_t i=0;i<10;i++)  
    {
      if(ValueG[i]!=0)
      {
        *PORTSG[i] &= ~BITSG[i];//low
      }
    }  
    ResetT0;
    for(uint8_t i=0;i<10;i++)
    {
      uint8_t trigger = ValueG[i];
      while(currTick0<trigger)
      {
      }
      *PORTSG[i] |= BITSG[i];
    }
    PORTF = 0;
  }
  {
    PORTF = _BV(6);
    //turn on
    for(uint8_t i=0;i<10;i++)  
    {
      if(ValueB[i]!=0)
      {
        *PORTSB[i] &= ~BITSB[i];//low
      }
    }  
    ResetT0;
    for(uint8_t i=0;i<10;i++)
    {
      uint8_t trigger = ValueB[i];
      while(currTick0<trigger)
      {
      }
      *PORTSB[i] |= BITSB[i];
    }
    PORTF = 0;
  }
}

void SortR(uint8_t startPos, uint8_t endPos) 
{ 
  uint8_t i;
  uint8_t j; 
  uint8_t key = ValueR[startPos]; 
  volatile uint8_t* v1 = PORTSR[startPos];
  uint8_t v2 = BITSR[startPos];
  i=startPos; 
  j=endPos; 
  while(i<j) 
  { 
    while(ValueR[j]>=key && i<j)--j; 
    ValueR[i] = ValueR[j];
    PORTSR[i] = PORTSR[j];
    BITSR[i] = BITSR[j];
    while(ValueR[i]<=key && i<j)++i; 
    ValueR[j] = ValueR[i]; 
    PORTSR[j] = PORTSR[i];
    BITSR[j] = BITSR[i];
  } 
  ValueR[i] = key; 
  PORTSR[i] = v1;
  BITSR[i] = v2;
  if(i-1>startPos) SortR(startPos,i-1); 
  if(endPos>i+1) SortR(i+1,endPos); 
} 

void SortG(uint8_t startPos, uint8_t endPos) 
{ 
  uint8_t i;
  uint8_t j; 
  uint8_t key = ValueG[startPos]; 
  volatile uint8_t* v1 = PORTSG[startPos];
  uint8_t v2 = BITSG[startPos];
  i=startPos; 
  j=endPos; 
  while(i<j) 
  { 
    while(ValueG[j]>=key && i<j)--j; 
    ValueG[i] = ValueG[j];
    PORTSG[i] = PORTSG[j];
    BITSG[i] = BITSG[j];
    while(ValueG[i]<=key && i<j)++i; 
    ValueG[j] = ValueG[i]; 
    PORTSG[j] = PORTSG[i];
    BITSG[j] = BITSG[i];
  } 
  ValueG[i] = key; 
  PORTSG[i] = v1;
  BITSG[i] = v2;
  if(i-1>startPos) SortG(startPos,i-1); 
  if(endPos>i+1) SortG(i+1,endPos); 
} 

void SortB(uint8_t startPos, uint8_t endPos) 
{ 
  uint8_t i;
  uint8_t j; 
  uint8_t key = ValueB[startPos]; 
  volatile uint8_t* v1 = PORTSB[startPos];
  uint8_t v2 = BITSB[startPos];
  i=startPos; 
  j=endPos; 
  while(i<j) 
  { 
    while(ValueB[j]>=key && i<j)--j; 
    ValueB[i] = ValueB[j];
    PORTSB[i] = PORTSB[j];
    BITSB[i] = BITSB[j];
    while(ValueB[i]<=key && i<j)++i; 
    ValueB[j] = ValueB[i]; 
    PORTSB[j] = PORTSB[i];
    BITSB[j] = BITSB[i];
  } 
  ValueB[i] = key; 
  PORTSB[i] = v1;
  BITSB[i] = v2;
  if(i-1>startPos) SortB(startPos,i-1); 
  if(endPos>i+1) SortB(i+1,endPos); 
} 




