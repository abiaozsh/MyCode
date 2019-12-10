/*
328p
8io data 	pin2~pin9
in  rdy		a0
out 5v		a1
out 12v		a2

595 10 11 12 13

out clockPB6	595 0
out !oe  	595 1
out !wr		595 2
out bs1		595 3
out xa0		595 4
out xa1		595 5
out pagel	595 6
out bs2		595 7
*/

#define PORT_DAT_ON  PORTB |=  _BV(2)
#define PORT_DAT_OFF PORTB &= ~_BV(2)

#define PORT_CLK_ON  PORTB |=  _BV(5)
#define PORT_CLK_OFF PORTB &= ~_BV(5)

#define PORT_STR_ON  PORTB |=  _BV(4)
#define PORT_STR_OFF PORTB &= ~_BV(4)

#define PORT_OE_HI   PORTB |=  _BV(3)
#define PORT_OE_LO   PORTB &= ~_BV(3)

#define  VCC      A1
#define  V12      A2

#define  RDY      A0

#define D0 9
#define D1 8
#define D2 7
#define D3 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2

uint8_t OE = 0;
uint8_t WR = 0;
uint8_t BS1 = 0;
uint8_t BS2 = 0;
uint8_t XTAL1 = 0;
uint8_t XA0 = 0;
uint8_t XA1 = 0;
uint8_t PAGEL = 0;

void outData(uint8_t data)
{
  digitalWrite(D0, ((data & 1) == 1)?HIGH:LOW);
  digitalWrite(D1, ((data & 2) == 2)?HIGH:LOW);
  digitalWrite(D2, ((data & 4) == 4)?HIGH:LOW);
  digitalWrite(D3, ((data & 8) == 8)?HIGH:LOW);
  digitalWrite(D4, ((data & 16) == 16)?HIGH:LOW);
  digitalWrite(D5, ((data & 32) == 32)?HIGH:LOW);
  digitalWrite(D6, ((data & 64) == 64)?HIGH:LOW);
  digitalWrite(D7, ((data & 128) == 128)?HIGH:LOW);
}
uint8_t inData()
{
  uint8_t data = 0;
  data |= (digitalRead(D0)==1)?1:0;
  data |= (digitalRead(D1)==1)?2:0;
  data |= (digitalRead(D2)==1)?4:0;
  data |= (digitalRead(D3)==1)?8:0;
  data |= (digitalRead(D4)==1)?16:0;
  data |= (digitalRead(D5)==1)?32:0;
  data |= (digitalRead(D6)==1)?64:0;
  data |= (digitalRead(D7)==1)?128:0;
  return data;
}

// low bit first
void SendByte(){
  uint8_t data = 0;
  data |= (OE==1)?1:0;
  data |= (WR==1)?2:0;
  data |= (BS1==1)?4:0;
  data |= (XTAL1==1)?8:0;
  data |= (XA0==1)?16:0;
  data |= (XA1==1)?32:0;
  data |= (PAGEL==1)?64:0;
  data |= (BS2==1)?128:0;
  
  for(uint8_t i=0;i<8;i++)
  {
    if(data&1)
    {
      PORT_DAT_ON;
    }
    else
    {
      PORT_DAT_OFF;
    }
    delayMicroseconds(10);
    data>>=1;
    PORT_CLK_ON; //shift clock up
    delayMicroseconds(10);
    PORT_CLK_OFF; //shift clock down
    delayMicroseconds(10);
  }
  PORT_STR_ON;
  delayMicroseconds(10);
  PORT_STR_OFF;
  delayMicroseconds(10);
}
void setup()
{
  digitalWrite(VCC, HIGH);//low: on           high:off
  pinMode(VCC, OUTPUT);
  digitalWrite(V12, LOW);//low: on           high:off
  pinMode(V12, OUTPUT);

  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);
  
  DDRB |= _BV(2);
  DDRB |= _BV(3);
  DDRB |= _BV(4);
  DDRB |= _BV(5);
  PORT_OE_HI;
  
  SendByte();
  
  PORT_OE_LO;
  
  // start serial port at 9600 bps:
  Serial.begin(115200);
}

void allout(){
    pinMode(D0, OUTPUT);
    pinMode(D1, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D3, OUTPUT);
    pinMode(D4, OUTPUT);
    pinMode(D5, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
}
void allin(){
    pinMode(D0, INPUT);
    pinMode(D1, INPUT);
    pinMode(D2, INPUT);
    pinMode(D3, INPUT);
    pinMode(D4, INPUT);
    pinMode(D5, INPUT);
    pinMode(D6, INPUT);
    pinMode(D7, INPUT);
}

void loadCommand(uint8_t command){
    //port.Write("z");// data output mode
    allout();
    //port.Write("j");// bs1 lo
    BS1=0;
    BS2=0;
    SendByte();
    //port.Write("e08");//��0000 1000��/e xx give data
    outData(command);
    //port.Write("n");// xa0 lo
    XA0=0;
    //port.Write("q");// xa1 hi
    XA1=1;SendByte();
    //port.Write("m");// xtal1 hi
    XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    XTAL1=0;SendByte();
    //port.Write("y");// data input mode
    allin();
}
void loadAddressLow(uint8_t address){
    //B. Load Address Low byte
    //1. Set XA1, XA0 to “00”. This enables address loading
    //2. Set BS1 to “0”. This selects low address
    //3. Set DATA = Address Low byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the address Low byte
    
    //port.Write("z");// data output mode
    allout();
    //port.Write("j");// bs1 lo
    BS1=0;
    BS2=0;
    SendByte();
    //port.Write("e00");//��0000 0000��/e xx give data
    outData(address);
    //port.Write("n");// xa0 lo
    XA0=0;
    //port.Write("p");// xa1 lo
    XA1=0;SendByte();
    //port.Write("m");// xtal1 hi
    XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    XTAL1=0;SendByte();
    //port.Write("y");// data input mode
    allin();
}
void loadAddressHigh(uint8_t address){
    //G. Load Address High byte
    //1. Set XA1, XA0 to “00”. This enables address loading
    //2. Set BS1 to “1”. This selects high address
    //3. Set DATA = Address High byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the address High byte
    
    //port.Write("z");// data output mode
    allout();
    //port.Write("j");// bs1 lo
    BS1=1;
    BS2=0;
    SendByte();
    //port.Write("e00");//��0000 0000��/e xx give data
    outData(address);
    //port.Write("n");// xa0 lo
    XA0=0;
    //port.Write("p");// xa1 lo
    XA1=0;SendByte();
    //port.Write("m");// xtal1 hi
    XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    XTAL1=0;SendByte();
    //port.Write("y");// data input mode
    allin();
}

void loadDataLow(uint8_t data){
    //C. Load Data Low byte
    //1. Set XA1, XA0 to “01”. This enables data loading
    //2. Set DATA = Data Low byte (0x00 - 0xFF)
    //3. Give XTAL1 a positive pulse. This loads the data byte

    //port.Write("z");// data output mode
    allout();
    //port.Write("j");// bs1 lo
    BS1=0;
    BS2=0;SendByte();
    //port.Write("e00");//��0000 0000��/e xx give data
    outData(data);
    //
    XA0=1;
    //port.Write("p");// xa1 lo
    XA1=0;SendByte();
    //port.Write("m");// xtal1 hi
    XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    XTAL1=0;SendByte();
    //port.Write("y");// data input mode
    allin();
}
void loadDataHigh(uint8_t data){
    //D. Load Data High byte
    //1. Set BS1 to “1”. This selects high data byte
    //2. Set XA1, XA0 to “01”. This enables data loading
    //3. Set DATA = Data High byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the data byte

    allout();
    BS1=1;
    BS2=0;
    XA0=1;
    //port.Write("p");// xa1 lo
    XA1=0;SendByte();
    outData(data);
    //port.Write("m");// xtal1 hi
    XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    XTAL1=0;SendByte();
    //port.Write("y");// data input mode
    allin();
}
/*
void readData(uint8_t bs1,uint8_t bs2){
    uint8_t data;
    //port.Write("f");// oe lo
    OE=0;
    //port.Write("j");// bs1 lo
    BS1=bs1;
    BS2=bs2;
    //port.Write("o");// xa0 hi
    XA0=1;
    //port.Write("q");// xa1 hi
    XA1=1;SendByte();
    //port.Write("m");// xtal1 hi
    XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    XTAL1=0;SendByte();
    delay(1);
    //port.Write("w");// getdata
    data = inData();
    printHex(data);
    
    //4. Set OE to ��1��
    //port.Write("g");// oe hi
    OE=1;SendByte();
}*/

void readData(uint8_t bs1,uint8_t bs2){
    uint8_t data;
    //port.Write("f");// oe lo
    OE=0;
    //port.Write("j");// bs1 lo
    BS1=bs1;
    BS2=bs2;
    //port.Write("o");// xa0 hi
    //XA0=1;
    //port.Write("q");// xa1 hi
    //XA1=1;
    SendByte();
    
    //port.Write("m");// xtal1 hi
    //XTAL1=1;SendByte();
    //port.Write("l");// xtal1 lo
    //XTAL1=0;SendByte();
    delay(1);
    //port.Write("w");// getdata
    data = inData();
    printHex(data);
    
    //4. Set OE to ��1��
    //port.Write("g");// oe hi
    OE=1;SendByte();
}
void powerON(){
    XTAL1=0;//port.Write("l");// xtal1 lo
    OE=1;//port.Write("g");// oe hi
    WR=1;//port.Write("i");// wr hi
    BS2=0;//port.Write("t");// bs2 lo
    SendByte();

    //1. Set Prog_enable pins listed in Table 92 on page 220 to ��0000��
    //port.Write("r");// pagel lo
    PAGEL=0;
    //port.Write("p");// xa1 lo
    XA1=0;
    //port.Write("n");// xa0 lo
    XA0=0;
    //port.Write("j");// bs1 lo
    BS1=0;
    SendByte();
    
    //2. Apply 4.5V - 5.5V between VCC and GND simultaneously as 11.5V - 12.5V is applied to RESET
    //port.Write("x");// vcc * v12 on
    PORTC &= ~_BV(1);
    PORTC |=  _BV(2);
    
    //3. Wait 100ns
    delay(1);
}
void powerOFF(){
    digitalWrite(VCC, HIGH); 
    digitalWrite(V12, LOW);  
}
void doWrite(){
    WR=0;
    SendByte();
    WR=1;
    SendByte();
    //Wait until RDY/BSY goes high
    while(true){
      uint8_t data = digitalRead(RDY);
      if(data==1){break;}
    }
}
/*
1 poweron
2 sendpage
3 writepage
4 poweroff

1 poweron
2 readpage
3 receivepage
4 poweroff
*/

//read fUse
//strued

//write default fUse
//stwued
uint8_t bufferL[32];
uint8_t bufferH[32];

void loop()
{
  while(!Serial.available());
  int cmd1 = Serial.read();
  if(cmd1>='a'&&cmd1<='z'){}else{return;}
  while(!Serial.available());
  int cmd2 = Serial.read();
  if(cmd2>='a'&&cmd2<='z'){}else{return;}

       if(cmd1=='s' && cmd2=='t')       //st Start
  {
    powerON();
    Serial.print("OK");
  }
  else if(cmd1=='e' && cmd2=='d')       //ed End
  {
    powerOFF();
    Serial.print("OK");
  }
  else if(cmd1=='s' && cmd2=='i')       //si ReadSignatureBytes
  {
    //1. A: Load Command ��0000 1000��
    loadCommand(0x08);
    //2. B: Load Address Low byte (0x00 - 0x02)
    loadAddressLow(0x00);
    //3. Set OE to ��0��, and BS1 to ��0��. The selected Signature byte can now be read at DATA.
    readData(0,0);
    //2. B: Load Address Low byte (0x00 - 0x02)
    loadAddressLow(0x01);
    readData(0,0);
    loadAddressLow(0x02);
    readData(0,0);
  }
  else if(cmd1=='e' && cmd2=='r')       //er Erase
  {
    loadCommand(0x80);
    //5. Give WR a negative pulse. This starts the Chip Erase. RDY/BSY goes low
    doWrite();
    Serial.print("OK");
  }
  else if(cmd1=='r' && cmd2=='u')       //rf read fUse
  {
    //1. A: Load Command ��0000 0100��
    loadCommand(0x04);
    //2. Set OE to ��0��, BS2 to ��0��, and BS1 to ��0��. The status of the Fuse Low bits can now be read at DATA (��0�� means programmed)
    readData(0,0);
    //3. Set OE to ��0��, BS2 to ��1��, and BS1 to ��1��. The status of the Fuse High bits can now be read at DATA (��0�� means programmed)
    readData(1,1);
    //4. Set OE to ��0��, BS2 to ��0��, and BS1 to ��1��. The status of the Lock Bits can now be read at DATA (��0�� means programmed)
    readData(1,0);
    //5. Set OE to ��1��
  }
  else if(cmd1=='w' && cmd2=='u')       //wf write default fUse
  {
    //A: Load Command ��0100 0000��
    loadCommand(0x40);
    //2. C: Load Data Low byte. Bit n = ��0�� programs and bit n = ��1�� erases the Fuse bit
    //3. Set BS1 and BS2 to ��0��
    loadDataLow(0xE4);//0xE4 1110 0100  8Mhz
    //4. Give WR a negative pulse and wait for RDY/BSY to go high
    doWrite();

    //1. A: Load Command ��0100 0000��
    //loadCommand(0x40);
    //2. C: Load Data Low byte. Bit n = ��0�� programs and bit n = ��1�� erases the Fuse bit
    loadDataLow(0xD9);//0xD9 1101 1001
    //3. Set BS1 to ��1�� and BS2 to ��0��. This selects high data byte
    BS1=1;
    BS2=0;
    //4. Give WR a negative pulse and wait for RDY/BSY to go high
    //5. Set BS1 to ��0��. This selects low data byte
    doWrite();
    BS1=0;
    Serial.print("OK");
  }
  else if(cmd1=='w' && cmd2=='f')       //wf write flash
  {
    //A. Load Command “Write Flash”
    //1. Set XA1, XA0 to “10”. This enables command loading
    //2. Set BS1 to ”0”
    //3. Set DATA to “0001 0000”. This is the command for Write Flash
    //4. Give XTAL1 a positive pulse. This loads the command
    loadCommand(0x10);
    Serial.print("OK");
  }
  else if(cmd1=='p' && cmd2=='b')       //pb LoadFlashPageBuffer
  {
    uint8_t valal = GetByte();
    for(uint8_t i = 0;i<32;i++){
      bufferL[i] = GetByte();
      bufferH[i] = GetByte();
    }
    for(uint8_t i = 0;i<32;i++){
      uint8_t valdl = bufferL[i];
      uint8_t valdh = bufferH[i];
      //B. Load Address Low byte
      //1. Set XA1, XA0 to “00”. This enables address loading
      //2. Set BS1 to “0”. This selects low address
      //3. Set DATA = Address Low byte (0x00 - 0xFF)
      //4. Give XTAL1 a positive pulse. This loads the address Low byte
      loadAddressLow(valal);
      valal++;
      //C. Load Data Low byte
      //1. Set XA1, XA0 to “01”. This enables data loading
      //2. Set DATA = Data Low byte (0x00 - 0xFF)
      //3. Give XTAL1 a positive pulse. This loads the data byte
      loadDataLow(valdl);
      //D. Load Data High byte
      //1. Set BS1 to “1”. This selects high data byte
      //2. Set XA1, XA0 to “01”. This enables data loading
      //3. Set DATA = Data High byte (0x00 - 0xFF)
      //4. Give XTAL1 a positive pulse. This loads the data byte
      loadDataHigh(valdh);
      //E. Latch Data
      //1. Set BS1 to “1”. This selects high data byte
      //2. Give PAGEL a positive pulse. This latches the data bytes (see Figure 106 on page 224 for signal waveforms)    
      BS1=1;
      PAGEL=1;
      SendByte();
      PAGEL=0;
      SendByte();
    }
    Serial.print("OK");
  }
  else if(cmd1=='h' && cmd2=='a')       //ha LoadFlashHighAddress
  {
    uint8_t valah = GetByte();
    //G. Load Address High byte
    //1. Set XA1, XA0 to “00”. This enables address loading
    //2. Set BS1 to “1”. This selects high address
    //3. Set DATA = Address High byte (0x00 - 0xFF)
    //4. Give XTAL1 a positive pulse. This loads the address High byte
    loadAddressHigh(valah);
    
    //H. Program Page
    //1. Set BS1 = “0”
    BS1=0;
    //2. Give WR a negative pulse. This starts programming of the entire page of data. RDY/BSY
    //goes low
    //3. Wait until RDY/BSY goes high. (See Figure 106 on page 224 for signal waveforms)
    doWrite();
    
    Serial.print("OK");
  }
  else if(cmd1=='n' && cmd2=='o')       //no NOP
  {
    //J. End Page Programming
    //1. Set XA1, XA0 to “10”. This enables command loading
    //2. Set DATA to “0000 0000”. This is the command for No Operation
    //3. Give XTAL1 a positive pulse. This loads the command, and the internal write signals are reset  
    loadCommand(0x00);
    Serial.print("OK");
  }
  else if(cmd1=='r' && cmd2=='f')       //rf ReadFlash
  {
    //1. A: Load Command “0000 0010”
    loadCommand(0x02);
    Serial.print("OK");
  }
  else if(cmd1=='f' && cmd2=='b')       //fb ReadFlashLowAndHighBytes1
  {
    uint8_t valal = GetByte();
    uint8_t valah = GetByte();
    //2. G: Load Address High byte (0x00 - 0xFF)
    loadAddressHigh(valah);
    //3. B: Load Address Low byte (0x00 - 0xFF)
    loadAddressLow(valal);
    //4. Set OE to “0”, and BS1 to “0”. The Flash word Low byte can now be read at DATA
    readData(0,0);
    //5. Set BS1 to “1”. The Flash word High byte can now be read at DATA
    readData(1,0);
    //6. Set OE to “1”
  }
}

char convt[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void printHex(uint16_t val){//"ll"+"hh"
  Serial.print(convt[((val & 0xF0) >> 4)]);
  Serial.print(convt[((val & 0x0F))]);
  Serial.print(convt[((val & 0xF000) >> 12)]);
  Serial.print(convt[((val & 0x0F00) >> 8)]);
}
void printHex(uint8_t val){//"hl"
  Serial.print(convt[((val & 0xF0) >> 4)]);
  Serial.print(convt[((val & 0x0F))]);
}
uint8_t GetByte(){
  while(!Serial.available());
  int vh = ConvBCD(Serial.read());
  while(!Serial.available());
  int vl = ConvBCD(Serial.read());
  uint8_t val = (( vh << 4 ) & 0xF0 ) | (vl & 0x0F);
  return val;
}
uint8_t ConvBCD(uint8_t val){
  if(val>='0'&&val<='9')
  {
    val = val - '0';
  }
  else if(val>='a'&&val<='f')
  {
    val = val - 'a' + 10;
  }
  else if(val>='A'&&val<='F')
  {
    val = val - 'A' + 10;
  }
  return val;
}

