
// Desired fuse configuration
#define  HFUSE  0xDF
#define  LFUSE  0x62   


#define  INSTOUT  2    // Connect to Serial Instruction Input (A5 SII) Pin 6
#define  DATAIN   3    // Connect to Serial Data Output (A4 SDO) Pin 7
#define  VCC      4    // Connect to VCC Pin 8
#define  DATAOUT  5    // Connect to Serial Data Input (A6 SDI) Pin 5 
#define  CLKOUT   6    // Connect to Serial Clock Input (B0 SCI) Pin 2
#define  RST      7    // Output to level shifter for !RESET from transistor to Pin 1

//VCC 					GND
//B0 (SCI)				A0 (GND)
//B1  					A1 (GND)
//RST (12V)				A2 (GND)
//B2  					A3
//A7  					A4 (SDO)/SCL
//A6 (SDI) MOSI			A5 (SII)/MISO 



//VCC B0(SCI) A6(SDI) A5(SII) A4(SDO)



//A0 GND
//A1 GND
//A2 GND
//2->A5(SII)
//3->A4(SDO)
//4->VCC 1k
//5->A6(SDI)
//6->B0(SCI)
//7->NPN 1k

int inByte = 0;         // incoming serial byte Computer
int inData = 0;         // incoming serial byte AVR

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
  Serial.begin(9600);

  establishContact();  // send a byte to establish contact until receiver responds 

}


void loop()
{
  // if we get a valid byte, run:
  if (Serial.available() > 0) {
    // get incoming byte:
    inByte = Serial.read();
    Serial.println(byte(inByte));
    Serial.println("Entering programming Mode\n");

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

    if(inByte=='w')
    {
      writeFuses();
    }
    if(inByte=='q')
    {
      writeFuses0();
    } 
    if(inByte=='e')
    {
      writeFusesf();
    }    //Programming mode
    Erase();
    readFuses();

    Serial.println("Exiting programming Mode\n");
    digitalWrite(CLKOUT, LOW);
    digitalWrite(VCC, LOW);
    digitalWrite(RST, LOW);//HIGH   //Turn off 12v
  }
}


void establishContact() {
  while (Serial.available() <= 0) {
    Serial.println("Enter a character to continue");   // send an initial string
    delay(1000);
  }
}
void writeFuses(){

    //Write hfuse
    Serial.println("Writing hfuse");
    shiftOut2(0x40, 0x4C);
    shiftOut2(HFUSE, 0x2C);
    shiftOut2(0x00, 0x74);
    shiftOut2(0x00, 0x7C);

    //Write lfuse
    Serial.println("Writing lfuse\n");
    shiftOut2(0x40, 0x4C);
    shiftOut2(LFUSE, 0x2C);
    shiftOut2(0x00, 0x64);
    shiftOut2(0x00, 0x6C);

}

void writeFusesf(){

    //Write hfuse
    Serial.println("Writing hfuse");
    shiftOut2(0x40, 0x4C);
    shiftOut2(0xFF, 0x2C);
    shiftOut2(0x00, 0x74);
    shiftOut2(0x00, 0x7C);

    //Write lfuse
    Serial.println("Writing lfuse\n");
    shiftOut2(0x40, 0x4C);
    shiftOut2(0xFF, 0x2C);
    shiftOut2(0x00, 0x64);
    shiftOut2(0x00, 0x6C);

}
void writeFuses0(){

    //Write hfuse
    Serial.println("Writing hfuse");
    shiftOut2(0x40, 0x4C);
    shiftOut2(0, 0x2C);
    shiftOut2(0x00, 0x74);
    shiftOut2(0x00, 0x7C);

    //Write lfuse
    Serial.println("Writing lfuse\n");
    shiftOut2(0x40, 0x4C);
    shiftOut2(0, 0x2C);
    shiftOut2(0x00, 0x64);
    shiftOut2(0x00, 0x6C);

}

void readFuses(){
  //Read Signature Bytes
  //SDI 0_0000_1000_00 0x04
  //SII 0_0100_1100_00 0x4C
  //SDO x_xxxx_xxxx_xx
  //0_0000_00bb_00 0x?? 0,1,2
  //0_0000_1100_00 0x0c
  //x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1000_00 0x68
  //x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1100_00 0x6C
  //q_qqqq_qqqx_xx
  //Repeats Instr 2 4 for each
  //signature byte address.
  shiftOut2(0x08, 0x4C);
  shiftOut2(0x00, 0x0c);
  shiftOut2(0x00, 0x68);
  inData = shiftOut2(0x00, 0x6C);
  Serial.print("Signature Bytes 0 ");
  Serial.println(inData, HEX);
  shiftOut2(0x08, 0x4C);
  shiftOut2(0x01, 0x0c);
  shiftOut2(0x00, 0x68);
  inData = shiftOut2(0x00, 0x6C);
  Serial.print("Signature Bytes 1 ");
  Serial.println(inData, HEX);
  shiftOut2(0x08, 0x4C);
  shiftOut2(0x02, 0x0c);
  shiftOut2(0x00, 0x68);
  inData = shiftOut2(0x00, 0x6C);
  Serial.print("Signature Bytes 2 ");
  Serial.println(inData, HEX);
  Serial.println(); 


  //Read Fuse Low Bits
  //SDI 0_0000_0100_00 0x04
  //SII 0_0100_1100_00 0x4c
  //SDO x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1000_00 0x68
  //x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1100_00 0x6c
  //A_9876_543x_xx
  //Reading A - 3 = “0” means the
  //Fuse bit is programmed.
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x68);
  inData = shiftOut2(0x00, 0x6C);
  Serial.print("Fuse Low Bits ");
  Serial.println(inData, HEX);

  //Read Fuse High Bits
  //SDI 0_0000_0100_00 0x04
  //SII 0_0100_1100_00 0x4c
  //SDO x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0111_1010_00 0x7A
  //x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0111_1100_00 0x7C
  //I_HGFE_DCBx_xx
  //Reading F - B = “0” means the
  //Fuse bit is programmed.
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x7A);
  inData = shiftOut2(0x00, 0x7C);//(0x00, 0x7E);
  Serial.print("Fuse High Bits ");
  Serial.println(inData, HEX);

  //Read Fuse Extended Bits
  //SDI 0_0000_0100_00 0x04
  //SII 0_0100_1100_00 0x4C
  //SDO x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1010_00 0x6A
  //x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1110_00 0x6E
  //x_xxxx_xxJx_xx
  //Reading J = “0” means the
  //Fuse bit is programmed.
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x6A);
  inData = shiftOut2(0x00, 0x6E);
  Serial.print("Fuse Extended Bits ");
  Serial.println(inData, HEX);

  //Read Lock Bits
  //SDI 0_0000_0100_00 0x04
  //SII 0_0100_1100_00 0x4C
  //SDO x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0111_1000_00 0x78
  //x_xxxx_xxxx_xx
  //0_0000_0000_00 0x00
  //0_0110_1100_00 0x6C
  //x_xxxx_x21x_xx
  //Reading 2, 1 = “0” means the
  //Lock bit is programmed.
  shiftOut2(0x04, 0x4C);
  shiftOut2(0x00, 0x78);
  inData = shiftOut2(0x00, 0x6C);
  Serial.print("Lock Bits ");
  Serial.println(inData, HEX);
  Serial.println(); 

  

}

void Erase(){
//Chip Erase
//SDI
//SII
//SDO
//0_1000_0000_00
//0_0100_1100_00
//x_xxxx_xxxx_xx
//0_0000_0000_00
//0_0110_0100_00
//x_xxxx_xxxx_xx
//0_0000_0000_00
//0_0110_1100_00
//x_xxxx_xxxx_xx
//Wait after Instr.3 until SDO
//goes high for the Chip Erase
//cycle to finish.
  

}



int shiftOut2(byte val, byte val1)
{
  int i;
  int inBits = 0;
  //Wait until DATAIN goes high
  while (!digitalRead(DATAIN));

  //Start bit
  digitalWrite(DATAOUT, LOW);
  digitalWrite(INSTOUT, LOW);
  digitalWrite(CLKOUT, HIGH);
  digitalWrite(CLKOUT, LOW);

  for (i = 0; i < 8; i++)  {

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

  return inBits;
}




Chip Erase
SDI
SII
SDO
0_1000_0000_00
0_0100_1100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_0100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_1100_00
x_xxxx_xxxx_xx
Wait after Instr.3 until SDO
goes high for the Chip Erase
cycle to finish.
Load “Write
Flash”
Command
SDI
SII
SDO
0_0001_0000_00
0_0100_1100_00
x_xxxx_xxxx_xx
Enter Flash Programming code.
Load Flash
Page Buffer
SDI
SII
SDO
0_ bbbb_bbbb _00
0_0000_1100_00
x_xxxx_xxxx_xx
0_eeee_eeee_00
0_0010_1100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_1101_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_1100_00
x_xxxx_xxxx_xx
Repeat after Instr. 1 - 7until the
entire page buffer is filled or
until all data within the page is
filled.(2)
SDI
SII
SDO
0_dddd_dddd_00
0_0011_1100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0111_1101_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0111_1100_00
x_xxxx_xxxx_xx
Instr 5-7.
Load Flash
High Address
and Program
Page
SDI
SII
SDO
0_0000_000a_00
0_0001_1100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_0100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_1100_00
x_xxxx_xxxx_xx
Wait after Instr 3 until SDO
goes high. Repeat Instr. 2 - 3
for each loaded Flash Page
until the entire Flash or all data
is programmed. Repeat Instr. 1
for a new 256 byte page.(2)
171
8183F–AVR–06/12
ATtiny24A/44A/84A
Load “Read
Flash”
Command
SDI
SII
SDO
0_0000_0010_00
0_0100_1100_00
x_xxxx_xxxx_xx
Enter Flash Read mode.
Read Flash
Low and High
Bytes
SDI
SII
SDO
0_bbbb_bbbb_00
0_0000_1100_00
x_xxxx_xxxx_xx
0_0000_000a_00
0_0001_1100_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_1000_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0110_1100_00
q_qqqq_qqqx_xx
Repeat Instr. 1, 3 - 6 for each
new address. Repeat Instr. 2 for
a new 256 byte page.
SDI
SII
SDO
0_0000_0000_00
0_0111_1000_00
x_xxxx_xxxx_xx
0_0000_0000_00
0_0111_1100_00
p_pppp_pppx_xx
Instr 5 - 6.