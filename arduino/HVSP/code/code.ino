#define  INSTOUT  2    // Connect to Serial Instruction Input (A5 SII) Pin 6
#define  DATAIN   3    // Connect to Serial Data Output (A4 SDO) Pin 7
#define  VCC      4    // Connect to VCC Pin 8
#define  DATAOUT  5    // Connect to Serial Data Input (A6 SDI) Pin 5 
#define  CLKOUT   8    // 6 Connect to Serial Clock Input (B0 SCI) Pin 2
#define  RST      9    // 7 Output to level shifter for !RESET from transistor to Pin 1
#define  LED     13

//VCC 					GND
//B0 (SCI)				A0 (GND)
//B1  					A1 (GND)
//RST (12V)				A2 (GND)
//B2  					A3
//A7  					A4 (SDO)/SCL
//A6 (SDI) MOSI			A5 (SII)/MISO 


//VCC B0(SCI) A6(SDI) A5(SII) A4(SDO)


//The Flash is organized in pages, see “Page Size” on page 162. When programming the Flash,
//the program data is latched into a page buffer. This allows one page of program data to be programmed
//simultaneously. The following procedure describes how to program the entire Flash
//memory:
//1. Load Command “Write Flash” (see Table 19-16 on page 171).
//2. Load Flash Page Buffer.
//3. Load Flash High Address and Program Page. Wait after Instr. 3 until SDO goes high for the “Page Programming” cycle to finish.
//4. Repeat 2 through 3 until the entire Flash is programmed or until all data has been
//programmed.
//5. End Page Programming by Loading Command “No Operation”.
//When writing or reading serial data to the ATtiny24/44/84, data is clocked on the rising edge of
//the serial clock, see Figure 20-6 on page 184, Figure 19-3 on page 167 and Table 20-13 on
//page 184 for details.


//A0 GND
//A1 GND
//A2 GND
//2->A5(SII)
//3->A4(SDO)
//4->VCC 1k
//5->A6(SDI)
//6->B0(SCI)
//7->NPN 1k

void setup()
{
  // Set up control lines for HV parallel programming
  pinMode(VCC, OUTPUT);
  digitalWrite(VCC, LOW);
  pinMode(RST, OUTPUT);
  digitalWrite(RST, LOW);//HIGH  // Level shifter is inverting, this shuts off 12V
  pinMode(DATAOUT, OUTPUT);
  digitalWrite(DATAOUT, LOW);
  pinMode(INSTOUT, OUTPUT);
  digitalWrite(INSTOUT, LOW);
  pinMode(CLKOUT, OUTPUT);
  digitalWrite(CLKOUT, LOW);
  pinMode(DATAIN, OUTPUT);  // configured as input when in programming mode
  digitalWrite(DATAIN, LOW);

  // start serial port at 9600 bps:
  Serial.begin(115200);
}



void loop()
{
  while(!Serial.available());
  int cmd1 = Serial.read();
  if(cmd1>='a'&&cmd1<='z'){}else{return;}
  while(!Serial.available());
  int cmd2 = Serial.read();
  if(cmd2>='a'&&cmd2<='z'){}else{return;}

  if(cmd1=='t' && cmd2=='s')            //ts test
  {
    uint8_t val = GetByte();
    printHex(val);
  }
  else if(cmd1=='s' && cmd2=='t')       //st Start
  {
    Start();
    Serial.print("OK");
  }
  else if(cmd1=='e' && cmd2=='d')       //ed End
  {
    End();
    Serial.print("OK");
  }
  else if(cmd1=='e' && cmd2=='r')       //er Erase
  {
    Erase();
    Serial.print("OK");
  }
  else if(cmd1=='r' && cmd2=='h')       //rh ReadHighFuses
  {
    uint8_t data = ReadHighFuses();
    printHex(data);
  }
  else if(cmd1=='r' && cmd2=='l')       //rl ReadLowFuses
  {
    uint8_t data = ReadLowFuses();
    printHex(data);
  }
  else if(cmd1=='r' && cmd2=='e')       //re ReadExtendedBits
  {
    uint8_t data = ReadExtendedBits();
    printHex(data);
  }
  else if(cmd1=='r' && cmd2=='k')       //rk ReadLockBits
  {
    uint8_t data = ReadLockBits();
    printHex(data);
  }
  else if(cmd1=='s' && cmd2=='i')       //si ReadSignatureBytes
  {
    uint8_t data;
    data = ReadSignatureBytes(0);
    printHex(data);
    
    data = ReadSignatureBytes(1);
    printHex(data);
    
    data = ReadSignatureBytes(2);
    printHex(data);
  }
  else if(cmd1=='w' && cmd2=='l')       //wl WriteLowFuses
  {
    uint8_t val = GetByte();
    WriteLowFuses(val);
    Serial.print("OK");
  }
  else if(cmd1=='w' && cmd2=='h')       //wh WriteHighFuses
  {
    uint8_t val = GetByte();
    WriteHighFuses(val);
    Serial.print("OK");
  }
  else if(cmd1=='w' && cmd2=='e')       //we WriteExtendedBits
  {
    uint8_t val = GetByte();
    WriteExtendedBits(val);
    Serial.print("OK");
  }
  else if(cmd1=='w' && cmd2=='k')       //wk WriteLockBits
  {
    uint8_t val = GetByte();
    WriteLockBits(val);
    Serial.print("OK");
  }
  else if(cmd1=='w' && cmd2=='f')       //wf WriteFlash
  {
    WriteFlash();
    Serial.print("OK");
  }
  else if(cmd1=='p' && cmd2=='b')       //pb LoadFlashPageBuffer
  {
    uint8_t valal = GetByte();
    uint8_t valdl = GetByte();
    uint8_t valdh = GetByte();
    LoadFlashPageBuffer(valal, valdl, valdh);
    Serial.print("OK");
  }
  else if(cmd1=='h' && cmd2=='a')       //ha LoadFlashHighAddress
  {
    uint8_t valah = GetByte();
    LoadFlashHighAddress(valah);
    Serial.print("OK");
  }
  else if(cmd1=='r' && cmd2=='f')       //rf ReadFlash
  {
    ReadFlash();
    Serial.print("OK");
  }
  else if(cmd1=='f' && cmd2=='b')       //fb ReadFlashLowAndHighBytes1
  {
    uint8_t valal = GetByte();
    uint8_t valah = GetByte();
    uint16_t data = ReadFlashLowAndHighBytes(valal, valah);
    printHex(data);
  }
  else if(cmd1=='w' && cmd2=='p')       //wp WriteEEPROM
  {
    WriteEEPROM();
    Serial.print("OK");
  }
  else if(cmd1=='w' && cmd2=='b')       //wb WriteEEPROMByte
  {
    uint8_t addr = GetByte();
    uint8_t data = GetByte();
    WriteEEPROMByte(addr, data);
    Serial.print("OK");
  }
  else if(cmd1=='r' && cmd2=='p')       //rp ReadFlash
  {
    ReadEEPROM();
    Serial.print("OK");
  }
  else if(cmd1=='r' && cmd2=='b')       //rb ReadEEPROMByte
  {
    uint8_t addr = GetByte();
    uint8_t data = ReadEEPROMByte(addr);
    printHex(data);
  }
  else if(cmd1=='n' && cmd2=='o')       //no NOP
  {
    NOP();
    Serial.print("OK");
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
uint8_t shiftOut2(byte val, byte val1){
  digitalWrite(LED, HIGH);
  uint8_t inBits = 0;
  //Start bit
  digitalWrite(DATAOUT, LOW);
  digitalWrite(INSTOUT, LOW);
  digitalWrite(CLKOUT, HIGH);
  digitalWrite(CLKOUT, LOW);
  for (uint8_t i = 0; i < 8; i++)  {
    //msb first
    digitalWrite(DATAOUT, !!(val & (1 << (7 - i))));
    digitalWrite(INSTOUT, !!(val1 & (1 << (7 - i))));
    inBits <<=1;
    inBits |= digitalRead(DATAIN);
    digitalWrite(CLKOUT, HIGH);
    digitalWrite(CLKOUT, LOW);
  }
  //End bits
  digitalWrite(DATAOUT, LOW);
  digitalWrite(INSTOUT, LOW);
  digitalWrite(CLKOUT, HIGH);
  digitalWrite(CLKOUT, LOW);
  digitalWrite(CLKOUT, HIGH);
  digitalWrite(CLKOUT, LOW);
  
  digitalWrite(LED, LOW);
  return inBits;
}

void Start(){
  // Initialize pins to enter programming mode
  pinMode(DATAIN, OUTPUT);  //Temporary
  digitalWrite(DATAOUT, LOW);
  digitalWrite(INSTOUT, LOW);
  digitalWrite(DATAIN, LOW);
  digitalWrite(RST, LOW);//HIGH  // Level shifter is inverting, this shuts off 12V

  // Enter High-voltage Serial programming mode
  digitalWrite(VCC, HIGH);  // Apply VCC to start programming process
  delayMicroseconds(20);
  digitalWrite(RST, HIGH); //LOW  //Turn on 12v
  delayMicroseconds(10);
  pinMode(DATAIN, INPUT);   //Release DATAIN
  delayMicroseconds(300);
}
void End(){
  digitalWrite(CLKOUT, LOW);
  digitalWrite(VCC, LOW);
  digitalWrite(RST, LOW);//HIGH   //Turn off 12v
}
void NOP(){
  //Load “No Operation” Command
  //SDI  SII  SDO
  //0_0000_0000_00  0_0100_1100_00  x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x4C);
}
void Erase(){
  //Chip Erase
  //0_1000_0000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x80, 0x4C);
  //0_0000_0000_00 0_0110_0100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x64);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6C);
  //Wait after Instr.3 until SDO goes high for the Chip Erase cycle to finish.
  while(!digitalRead(DATAIN));//until SDO goes high.
  //Load Command “No Operation”.
  NOP();
}
uint8_t ReadLowFuses(){
  //Read Fuse Low Bits
  //0_0000_0100_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x04, 0x4C);
  //0_0000_0000_00 0_0110_1000_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x68);
  //0_0000_0000_00 0_0110_1100_00 A_9876_543x_xx
  return shiftOut2(0x00, 0x6C);
  //Reading A - 3 = “0” means the Fuse bit is programmed.
}
uint8_t ReadHighFuses(){
  //Read Fuse High Bits
  //0_0000_0100_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x04, 0x4C);
  //0_0000_0000_00 0_0111_1010_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x7A);
  //0_0000_0000_00 0_0111_1100_00 I_HGFE_DCBx_xx
  return shiftOut2(0x00, 0x7C);
  //Reading F - B = “0” means the Fuse bit is programmed.
}
uint8_t ReadExtendedBits(){
  //Read Fuse Extended Bits
  //0_0000_0100_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x04, 0x4C);
  //0_0000_0000_00 0_0110_1010_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6A);
  //0_0000_0000_00 0_0110_1110_00 x_xxxx_xxJx_xx
  return shiftOut2(0x00, 0x6E);
  //Reading J = “0” means the Fuse bit is programmed.
}
uint8_t ReadLockBits(){
  //Read Lock Bits
  //0_0000_0100_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x04, 0x4C);
  //0_0000_0000_00 0_0111_1000_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x78);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_x21x_xx
  return shiftOut2(0x00, 0x6C);
  //Reading 2, 1 = “0” means the Lock bit is programmed.
}
uint8_t ReadSignatureBytes(uint8_t i){
  //Read Signature Bytes
  //0_0000_1000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x08, 0x4C);
  //0_0000_00bb_00 0_0000_1100_00 x_xxxx_xxxx_xx
  shiftOut2(i, 0x0C);
  //0_0000_0000_00 0_0110_1000_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x68);
  //0_0000_0000_00 0_0110_1100_00 q_qqqq_qqqx_xx
  return shiftOut2(0x00, 0x6C);
  //Repeats Instr 2 4 for each signature byte address.
}
void WriteLowFuses(uint8_t val){
  //Write Fuse Low Bits
  //0_0100_0000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x40, 0x4C);
  //0_A987_6543_00 0_0010_1100_00 x_xxxx_xxxx_xx
  shiftOut2(val, 0x2C);
  //0_0000_0000_00 0_0110_0100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x64);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_xxxx_xx  
  shiftOut2(0x00, 0x6C);
  //Wait after Instr. 4 until SDO goes high. Write A - 3 = “0” to program the Fuse bit.
  while(!digitalRead(DATAIN));//until SDO goes high.
}
void WriteHighFuses(uint8_t val){
  //Write Fuse High Bits
  //0_0100_0000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x40, 0x4C);
  //0_IHGF_EDCB_00 0_0010_1100_00 x_xxxx_xxxx_xx
  shiftOut2(val, 0x2C);
  //0_0000_0000_00 0_0111_0100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x74);
  //0_0000_0000_00 0_0111_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x7C);
  //Wait after Instr. 4 until SDO goes high. Write F - B = “0” to program the Fuse bit.
  while(!digitalRead(DATAIN));//until SDO goes high.
}
void WriteExtendedBits(uint8_t val){
  //Write Fuse Extended Bits
  //0_0100_0000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x40, 0x4C);
  //0_0000_000J_00 0_0010_1100_00 x_xxxx_xxxx_xx
  shiftOut2(val, 0x2C);
  //0_0000_0000_00 0_0110_0110_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x66);
  //0_0000_0000_00 0_0110_1110_00 x_xxxx_xxxx_xx  
  shiftOut2(0x00, 0x6E);
  //Wait after Instr. 4 until SDO goes high. Write J = “0” to program the Fuse bit.
  while(!digitalRead(DATAIN));//until SDO goes high.
}
void WriteLockBits(uint8_t val){
  //Write Lock Bits
  //0_0010_0000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x20, 0x4C);
  //0_0000_0021_00 0_0010_1100_00 x_xxxx_xxxx_xx
  shiftOut2(val, 0x2C);
  //0_0000_0000_00 0_0110_0100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x64);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_xxxx_xx  
  shiftOut2(0x00, 0x6C);
  //Wait after Instr. 4 until SDO goes high. Write 2 - 1 = “0” to program the Lock Bit.
  while(!digitalRead(DATAIN));//until SDO goes high.
}
void WriteFlash(){
  //Load “Write Flash” Command
  //0_0001_0000_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x10, 0x4C);
  //Enter Flash Programming code.
}
void LoadFlashPageBuffer(uint8_t addLow, uint8_t dataLow, uint8_t dataHigh){
  //Load Flash Page Buffer
  //0_bbbb_bbbb_00 0_0000_1100_00 x_xxxx_xxxx_xx
  shiftOut2(addLow, 0x0C);
  //0_eeee_eeee_00 0_0010_1100_00 x_xxxx_xxxx_xx
  shiftOut2(dataLow, 0x2C);
  //0_0000_0000_00 0_0110_1101_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6D);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6C);
  //0_dddd_dddd_00 0_0011_1100_00 x_xxxx_xxxx_xx
  shiftOut2(dataHigh, 0x3C);
  //0_0000_0000_00 0_0111_1101_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x7D);
  //0_0000_0000_00 0_0111_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x7C);
  //Repeat after Instr. 1 - 7until the entire page buffer is filled or until all data within the page is filled.(2)
}
void LoadFlashHighAddress(uint8_t addHigh){
  //Load Flash High Address and Program Page
  //0_0000_000a_00 0_0001_1100_00 x_xxxx_xxxx_xx
  shiftOut2(addHigh, 0x1C);
  //0_0000_0000_00 0_0110_0100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x64);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6C);
  //Wait after Instr 3 until SDO goes high. Repeat Instr. 2 - 3 for each loaded Flash Page until the entire Flash or all data is programmed. Repeat Instr. 1 for a new 256 byte page.
  while(!digitalRead(DATAIN));//until SDO goes high.
}
void ReadFlash(){
  //Load “Read Flash” Command
  //0_0000_0010_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x02, 0x4C);
  //Enter Flash Read mode.
}
uint16_t ReadFlashLowAndHighBytes(uint8_t addLow, uint8_t addHigh){
  // Read Flash Low and High Bytes
  //0_bbbb_bbbb_00 0_0000_1100_00 x_xxxx_xxxx_xx
  shiftOut2(addLow, 0x0C);
  //0_0000_000a_00 0_0001_1100_00 x_xxxx_xxxx_xx
  shiftOut2(addHigh, 0x1C);
  //0_0000_0000_00 0_0110_1000_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x68);
  //0_0000_0000_00 0_0110_1100_00 q_qqqq_qqqx_xx
  uint16_t lowBits = shiftOut2(0x00, 0x6C);
  //0_0000_0000_00 0_0111_1000_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x78);
  //0_0000_0000_00 0_0111_1100_00 p_pppp_pppx_xx
  uint16_t highBits = shiftOut2(0x00, 0x7C);
  //Repeat Instr. 1, 3 - 6 for each new address. Repeat Instr. 2 for a new 256 byte page.
  return lowBits | (highBits << 8);
}

void WriteEEPROM(){
  //Load “Write EEPROM” Command
  //0_0001_0001_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x11, 0x4C);
  //Enter EEPROM Programming mode.
}

void WriteEEPROMByte(uint8_t address,uint8_t data){
  //Write EEPROM Byte
  //0_bbbb_bbbb_00 0_0000_1100_00 x_xxxx_xxxx_xx
  shiftOut2(address, 0x0C);
  //0_aaaa_aaaa_00 0_0001_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x1C);
  //0_eeee_eeee_00 0_0010_1100_00 x_xxxx_xxxx_xx
  shiftOut2(data, 0x2C);
  //0_0000_0000_00 0_0110_1101_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6D);
  //Repeat Instr. 1 - 6 for each new address. Wait after Instr. 6 until SDO goes high.(4)
  //0_0000_0000_00 0_0110_0100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x64);
  //0_0000_0000_00 0_0110_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x6C);
}

void ReadEEPROM(){
  //Load “Read EEPROM” Command
  //0_0000_0011_00 0_0100_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x03, 0x4C);
  //Enter EEPROM Read mode.
}

uint8_t ReadEEPROMByte(uint8_t address){
  //Read EEPROM Byte
  //0_bbbb_bbbb_00 0_0000_1100_00 x_xxxx_xxxx_xx
  shiftOut2(address, 0x0C);
  //0_aaaa_aaaa_00 0_0001_1100_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x1C);
  //0_0000_0000_00 0_0110_1000_00 x_xxxx_xxxx_xx
  shiftOut2(0x00, 0x68);
  //0_0000_0000_00 0_0110_1100_00 q_qqqq_qqq0_00
  uint8_t data = shiftOut2(0x00, 0x6C);
  //Repeat Instr. 1, 3 - 4 for each new address. Repeat Instr. 2 for a new 256 byte page.
  return data;
}
