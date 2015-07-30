/*
GENERAL DESCRIPTION
 The 74HC/HCT165 are high-speed Si-gate CMOS devices
 and are pin compatible with low power Schottky TTL
 (LSTTL). They are specified in compliance with JEDEC
 standard no. 7A.
 The 74HC/HCT165 are 8-bit parallel-load or serial-in shift
 registers with complementary serial outputs (Q7 and
 Q7) available from the last stage. When the parallel load
 (PL) input is LOW, parallel data from the D0 to
 D7 inputs are loaded into the register asynchronously.
 When PL is HIGH, data enters the register serially at the
 Ds input and shifts one place to the right
 (Q0 ® Q1 ® Q2, etc.) with each positive-going clock
 transition. This feature allows parallel-to-serial converter
 expansion by tying the Q7 output to the DS input of the
 succeeding stage.
 The clock input is a gated-OR structure which allows one
 input to be used as an active LOW clock enable (CE) input.
 The pin assignment for the CP and CE inputs is arbitrary
 and can be reversed for layout convenience. The
 LOW-to-HIGH transition of input CE should only take
 place while CP HIGH for predictable operation. Either the
 CP or the CE should be HIGH before the
 LOW-to-HIGH transition of PL to prevent shifting the data
 when PL is activated.
 
 ~PL(IN)    VCC
 CP(IN)     ~CE(GND)
 D4         D3
 D5         D2
 D6         D1
 D7         D0
 ~Q7(free)  DS(free)
 GND        Q7(out)
 
 PIN DESCRIPTION
 PIN NO. SYMBOL NAME AND FUNCTION
 1 PL asynchronous parallel load input (active LOW)
 (free)7 Q7 complementary output from the last stage
 (out)9 Q7 serial output from the last stage
 (clk/GND)2 CP clock input (LOW-to-HIGH edge-triggered)
 8 GND ground (0 V)
 GND 10 Ds serial data input
 11, 12, 13, 14, 3, 4, 5, 6 D0 to D7 parallel data inputs
 (clk/GND)15 CE clock enable input (active LOW)
 16 VCC positive supply voltage
 Fig.
 
 */

#define PL_ON  PORTD |=  _BV(2)
#define PL_OFF PORTD &= ~_BV(2)

#define CP_ON  PORTD |=  _BV(3)
#define CP_OFF PORTD &= ~_BV(3)

#define PINQ7  (PIND & _BV(4))

void setup()
{
  DDRD |= _BV(2);//pin9 B1 Shift Clock Pin 11
  DDRD |= _BV(3);//pin8 B0 Store Clock Pin 12

  Serial.begin(9600);

  while(true)
  {
    uint8_t data = get165();
    Serial.println(data);
    delay(1000);
  }
}
uint8_t get165()
{
  uint8_t data = 0;
  PL_OFF;
  PL_ON;
  for(uint8_t i=0;i<8;i++)
  {
    data = data << 1;
    if(PINQ7){
      data|=1;
    }
    CP_ON;
    CP_OFF;
  }
  return data;
}
void loop()
{

}


