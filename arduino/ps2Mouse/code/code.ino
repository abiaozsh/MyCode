    int _clock_pin;
    int _data_pin;


void write(uint16_t data) {
  char i;
  pull_high(_clock_pin);
  
  delayMicroseconds(100);
  pull_low(_data_pin);
  pull_low(_clock_pin);
  
  delayMicroseconds(100);
  pull_high(_clock_pin); // Start Bit
  
  // clock is low, and we are clear to send data
  for (i=0; i < 10; i++) {
    if (data & 0x01) {
      pull_high(_data_pin);
    } else {
      pull_low(_data_pin);
    }
    // wait for clock cycle
    while (!digitalRead(_clock_pin)) {;}
    while (digitalRead(_clock_pin)) {;}
    data = data >> 1;
  }
  while ((!digitalRead(_clock_pin)) || (!digitalRead(_data_pin))) {;} // wait for mouse to switch modes
  pull_low(_clock_pin); // put a hold on the incoming data.
}

int8_t read_byte() {
  int8_t data = 0;
  pull_high(_clock_pin);
  pull_high(_data_pin);
  delayMicroseconds(50);
  for (int i = 0; i < 11; i++) {
    while (digitalRead(_clock_pin)) {;}
    int bit = digitalRead(_data_pin);
    while (!digitalRead(_clock_pin)) {;}
    if(bit){
      data |= 1<<i;
    }
  }
  pull_low(_clock_pin);
  return data;
}


int16_t read_movement_x(int status) {
  int16_t x = (read_byte()>>1)&0xFF;
  if (bitRead(status, 4)) {
    for(int i = 8; i < 16; ++i) {
      x |= (1<<i);
    }
  }
  return x;
}

int16_t read_movement_y(int status) {
  int16_t y = (read_byte()>>1)&0xFF;
  if (bitRead(status, 5)) {
    for(int i = 8; i < 16; ++i) {
      y |= (1<<i);
    }
  }
  return y;
}

void pull_low(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void pull_high(int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

//v
//g
//...
//clk
//data

//白色
#define MOUSE_DATA 2
//绿色
#define MOUSE_CLOCK 3

/**
 * Setup
 */
void setup()
{
  Serial.begin(9600);
  _clock_pin = MOUSE_CLOCK;
  _data_pin = MOUSE_DATA;

}

/**
 * Main program loop
 */
void loop()
{
  int16_t data[4];
  
  write(0x03D6); // Send Read Data

  data[0] = read_byte(); // Status bit
  data[1] = (read_byte()>>1)&0xFF; // Status bit
  data[2] = read_movement_x(data[1]); // X Movement Packet
  data[3] = read_movement_y(data[1]); // Y Movement Packet

  Serial.print((data[1]>>7)&1); // Status Byte
  Serial.print((data[1]>>6)&1); // Status Byte
  Serial.print((data[1]>>5)&1); // Status Byte
  Serial.print((data[1]>>4)&1); // Status Byte
  Serial.print((data[1]>>3)&1); // Status Byte
  Serial.print((data[1]>>2)&1); // Status Byte
  Serial.print((data[1]>>1)&1); // Status Byte
  Serial.print((data[1]>>0)&1); // Status Byte
  Serial.print(":");
  Serial.print(data[2]); // X Movement Data
  Serial.print(",");
  Serial.print(data[3]); // Y Movement Data
  Serial.println();
  delay(1);
}
