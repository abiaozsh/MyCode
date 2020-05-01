#ifndef PS2Mouse_h
#define PS2Mouse_h
#define REMOTE 1
#define STREAM 2


// Every call to read() returns a single byte for each
// keystroke.  These configure what byte will be returned
// for each "special" key.  To ignore a key, use zero.
#define PS2_TAB				9
#define PS2_ENTER			13
#define PS2_BACKSPACE			127
#define PS2_ESC				27
#define PS2_INSERT			0
#define PS2_DELETE			127
#define PS2_HOME			0
#define PS2_END				0
#define PS2_PAGEUP			25
#define PS2_PAGEDOWN			26
#define PS2_UPARROW			11
#define PS2_LEFTARROW			8
#define PS2_DOWNARROW			10
#define PS2_RIGHTARROW			21
#define PS2_F1				0
#define PS2_F2				0
#define PS2_F3				0
#define PS2_F4				0
#define PS2_F5				0
#define PS2_F6				0
#define PS2_F7				0
#define PS2_F8				0
#define PS2_F9				0
#define PS2_F10				0
#define PS2_F11				0
#define PS2_F12				0
#define PS2_SCROLL			0

#define PS2_INVERTED_EXCLAMATION	161 // ¡
#define PS2_CENT_SIGN			162 // ¢
#define PS2_POUND_SIGN			163 // £
#define PS2_CURRENCY_SIGN		164 // ¤
#define PS2_YEN_SIGN			165 // ¥
#define PS2_BROKEN_BAR                 	166 // ¦
#define PS2_SECTION_SIGN		167 // §
#define PS2_DIAERESIS			168 // ¨
#define PS2_COPYRIGHT_SIGN		169 // ©
#define PS2_FEMININE_ORDINAL		170 // ª
#define PS2_LEFT_DOUBLE_ANGLE_QUOTE	171 // «
#define PS2_NOT_SIGN			172 // ¬
#define PS2_HYPHEN			173
#define PS2_REGISTERED_SIGN		174 // ®
#define PS2_MACRON			175 // ¯
#define PS2_DEGREE_SIGN			176 // °
#define PS2_PLUS_MINUS_SIGN		177 // ±
#define PS2_SUPERSCRIPT_TWO		178 // ²
#define PS2_SUPERSCRIPT_THREE		179 // ³
#define PS2_ACUTE_ACCENT		180 // ´
#define PS2_MICRO_SIGN			181 // µ
#define PS2_PILCROW_SIGN		182 // ¶
#define PS2_MIDDLE_DOT			183 // ·
#define PS2_CEDILLA			184 // ¸
#define PS2_SUPERSCRIPT_ONE		185 // ¹
#define PS2_MASCULINE_ORDINAL		186 // º
#define PS2_RIGHT_DOUBLE_ANGLE_QUOTE	187 // »
#define PS2_FRACTION_ONE_QUARTER	188 // ¼
#define PS2_FRACTION_ONE_HALF		189 // ½
#define PS2_FRACTION_THREE_QUARTERS	190 // ¾
#define PS2_INVERTED_QUESTION MARK	191 // ¿
#define PS2_A_GRAVE			192 // À
#define PS2_A_ACUTE			193 // Á
#define PS2_A_CIRCUMFLEX		194 // Â
#define PS2_A_TILDE			195 // Ã
#define PS2_A_DIAERESIS			196 // Ä
#define PS2_A_RING_ABOVE		197 // Å
#define PS2_AE				198 // Æ
#define PS2_C_CEDILLA			199 // Ç
#define PS2_E_GRAVE			200 // È
#define PS2_E_ACUTE			201 // É
#define PS2_E_CIRCUMFLEX		202 // Ê
#define PS2_E_DIAERESIS			203 // Ë
#define PS2_I_GRAVE			204 // Ì
#define PS2_I_ACUTE			205 // Í
#define PS2_I_CIRCUMFLEX		206 // Î
#define PS2_I_DIAERESIS			207 // Ï
#define PS2_ETH				208 // Ð
#define PS2_N_TILDE			209 // Ñ
#define PS2_O_GRAVE			210 // Ò
#define PS2_O_ACUTE			211 // Ó
#define PS2_O_CIRCUMFLEX		212 // Ô
#define PS2_O_TILDE			213 // Õ
#define PS2_O_DIAERESIS			214 // Ö
#define PS2_MULTIPLICATION		215 // ×
#define PS2_O_STROKE			216 // Ø
#define PS2_U_GRAVE			217 // Ù
#define PS2_U_ACUTE			218 // Ú
#define PS2_U_CIRCUMFLEX		219 // Û
#define PS2_U_DIAERESIS			220 // Ü
#define PS2_Y_ACUTE			221 // Ý
#define PS2_THORN			222 // Þ
#define PS2_SHARP_S			223 // ß
#define PS2_a_GRAVE			224 // à
#define PS2_a_ACUTE			225 // á
#define PS2_a_CIRCUMFLEX		226 // â
#define PS2_a_TILDE			227 // ã
#define PS2_a_DIAERESIS			228 // ä
#define PS2_a_RING_ABOVE		229 // å
#define PS2_ae				230 // æ
#define PS2_c_CEDILLA			231 // ç
#define PS2_e_GRAVE			232 // è
#define PS2_e_ACUTE			233 // é
#define PS2_e_CIRCUMFLEX		234 // ê
#define PS2_e_DIAERESIS			235 // ë
#define PS2_i_GRAVE			236 // ì
#define PS2_i_ACUTE			237 // í
#define PS2_i_CIRCUMFLEX		238 // î
#define PS2_i_DIAERESIS			239 // ï
#define PS2_eth				240 // ð
#define PS2_n_TILDE			241 // ñ
#define PS2_o_GRAVE			242 // ò
#define PS2_o_ACUTE			243 // ó
#define PS2_o_CIRCUMFLEX		244 // ô
#define PS2_o_TILDE			245 // õ
#define PS2_o_DIAERESIS			246 // ö
#define PS2_DIVISION			247 // ÷
#define PS2_o_STROKE			248 // ø
#define PS2_u_GRAVE			249 // ù
#define PS2_u_ACUTE			250 // ú
#define PS2_u_CIRCUMFLEX		251 // û
#define PS2_u_DIAERESIS			252 // ü
#define PS2_y_ACUTE			253 // ý
#define PS2_thorn			254 // þ
#define PS2_y_DIAERESIS			255 // ÿ

class PS2Mouse
{
  public:
    int _clock_pin;
    int _data_pin;
    int _mode;
    int _initialized;
    int _enabled;
    int _disabled;
    int8_t read_byte();
    int read_bit();
    int16_t read_movement_x(int);
    int16_t read_movement_y(int);
    void pull_high(int);
    void pull_low(int);
    void set_mode(int);

    PS2Mouse(int, int, int mode = REMOTE);
    void initialize();
    int clock_pin();
    int data_pin();
    int read();
    int16_t* report(int16_t data[]);
    void write(int);
    void enable_data_reporting();
    void disable_data_reporting();
    void set_remote_mode();
    void set_stream_mode();
    void set_resolution(int);
    void set_scaling_2_1();
    void set_scaling_1_1();
    void set_sample_rate(int);
};

#endif


#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WConstants.h"
#endif

#include "HardwareSerial.h"

PS2Mouse::PS2Mouse(int clock_pin, int data_pin, int mode) {
  _clock_pin = clock_pin;
  _data_pin = data_pin;
  _mode = mode;
  _initialized = false;
  _disabled = true;
  _enabled = false;
}

int PS2Mouse::clock_pin() {
  return _clock_pin;
}

int PS2Mouse::data_pin() {
  return _data_pin;
}

void PS2Mouse::initialize() {
  pull_high(_clock_pin);
  pull_high(_data_pin);
  delay(20);
  write(0xff); // Send Reset to the mouse
  read_byte();  // Read ack byte
  delay(20); // Not sure why this needs the delay
  read_byte();  // blank
  read_byte();  // blank
  delay(20); // Not sure why this needs the delay
  if (_mode == REMOTE) {
    set_remote_mode();
  } else {
    enable_data_reporting(); // Tell the mouse to start sending data again
  }
  delayMicroseconds(100);
  _initialized = 1;
}

void PS2Mouse::set_mode(int data) {
  if (_mode == STREAM) {
    disable_data_reporting(); // Tell the mouse to stop sending data.
  }
  write(data);  // Send Set Mode
  read_byte();  // Read Ack byte
  if (_mode == STREAM) {
    enable_data_reporting(); // Tell the mouse to start sending data again
  }
  if (_initialized) {
    delayMicroseconds(100);
  }
}

void PS2Mouse::set_remote_mode() {
  set_mode(0xf0);
  _mode = REMOTE;
}

void PS2Mouse::set_stream_mode() {
  set_mode(0xea);
  _mode = STREAM;
}

void PS2Mouse::set_sample_rate(int rate) {
  if (_mode == STREAM) {
    disable_data_reporting(); // Tell the mouse to stop sending data.
  }
  write(0xf3); // Tell the mouse we are going to set the sample rate.
  read_byte(); // Read Ack Byte
  write(rate); // Send Set Sample Rate
  read_byte(); // Read ack byte
  if (_mode == STREAM) {
    enable_data_reporting(); // Tell the mouse to start sending data again
  }
  delayMicroseconds(100);
}

void PS2Mouse::set_scaling_2_1() {
  set_mode(0xe7); // Set the scaling to 2:1
}

void PS2Mouse::set_scaling_1_1() {
  set_mode(0xe6); // set the scaling to 1:1
}

// This only effects data reporting in Stream mode.
void PS2Mouse::enable_data_reporting() {
  if (!_enabled) {
    write(0xf4); // Send enable data reporting
    read_byte(); // Read Ack Byte
    _enabled = true;
  }
}

// Disabling data reporting in Stream Mode will make it behave like Remote Mode
void PS2Mouse::disable_data_reporting() {
  if (!_disabled) {
    write(0xf5); // Send disable data reporting
    read_byte(); // Read Ack Byte
    _disabled = true;
  }
}

void PS2Mouse::set_resolution(int resolution) {
  if (_mode == STREAM) {
    enable_data_reporting();
  }
  write(0xe8); // Send Set Resolution
  read_byte(); // Read ack Byte
  write(resolution); // Send resolution setting
  read_byte(); // Read ack Byte
  if (_mode == STREAM) {
    disable_data_reporting();
  }
  delayMicroseconds(100);
}

void PS2Mouse::write(int data) {
  char i;
  char parity = 1;
  pull_high(_data_pin);
  pull_high(_clock_pin);
  delayMicroseconds(300);
  pull_low(_clock_pin);
  delayMicroseconds(300);
  pull_low(_data_pin);
  delayMicroseconds(10);
  pull_high(_clock_pin); // Start Bit
  while (digitalRead(_clock_pin)) {;} // wait for mouse to take control of clock)
  // clock is low, and we are clear to send data
  for (i=0; i < 8; i++) {
    if (data & 0x01) {
      pull_high(_data_pin);
    } else {
      pull_low(_data_pin);
    }
    // wait for clock cycle
    while (!digitalRead(_clock_pin)) {;}
    while (digitalRead(_clock_pin)) {;}
    parity = parity ^ (data & 0x01);
    data = data >> 1;
  }
  // parity
  if (parity) {
    pull_high(_data_pin);
  } else {
    pull_low(_data_pin);
  }
  while (!digitalRead(_clock_pin)) {;}
  while (digitalRead(_clock_pin)) {;}
  pull_high(_data_pin);
  delayMicroseconds(50);
  while (digitalRead(_clock_pin)) {;}
  while ((!digitalRead(_clock_pin)) || (!digitalRead(_data_pin))) {;} // wait for mouse to switch modes
  pull_low(_clock_pin); // put a hold on the incoming data.
}

int16_t * PS2Mouse::report(int16_t data[]) {
  write(0xeb); // Send Read Data
  read_byte(); // Read Ack Byte
  data[0] = read(); // Status bit
  data[1] = read_movement_x(data[0]); // X Movement Packet
  data[2] = read_movement_y(data[0]); // Y Movement Packet
  return data;
}

int PS2Mouse::read() {
  return read_byte();
}

int8_t PS2Mouse::read_byte() {
  int8_t data = 0;
  pull_high(_clock_pin);
  pull_high(_data_pin);
  delayMicroseconds(50);
  while (digitalRead(_clock_pin)) {;}
  delayMicroseconds(5);  // not sure why.
  while (!digitalRead(_clock_pin)) {;} // eat start bit
  for (int i = 0; i < 8; i++) {
    bitWrite(data, i, read_bit());
  }
  read_bit(); // Partiy Bit
  read_bit(); // Stop bit should be 1
  pull_low(_clock_pin);
  return data;
}

int PS2Mouse::read_bit() {
  while (digitalRead(_clock_pin)) {;}
  int bit = digitalRead(_data_pin);
  while (!digitalRead(_clock_pin)) {;}
  return bit;
}

int16_t PS2Mouse::read_movement_x(int status) {
  int16_t x = read();
  if (bitRead(status, 4)) {
    for(int i = 8; i < 16; ++i) {
      x |= (1<<i);
    }
  }
  return x;
}

int16_t PS2Mouse::read_movement_y(int status) {
  int16_t y = read();
  if (bitRead(status, 5)) {
    for(int i = 8; i < 16; ++i) {
      y |= (1<<i);
    }
  }
  return y;
}

void PS2Mouse::pull_low(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2Mouse::pull_high(int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

const PROGMEM char PS2Keymap_US[] = {
  // without shift
	0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '`', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'q', '1', 0,
	0, 0, 'z', 's', 'a', 'w', '2', 0,
	0, 'c', 'x', 'd', 'e', '4', '3', 0,
	0, ' ', 'v', 'f', 't', 'r', '5', 0,
	0, 'n', 'b', 'h', 'g', 'y', '6', 0,
	0, 0, 'm', 'j', 'u', '7', '8', 0,
	0, ',', 'k', 'i', 'o', '0', '9', 0,
	0, '.', '/', 'l', ';', 'p', '-', 0,
	0, 0, '\'', 0, '[', '=', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, ']', 0, '\\', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 };

const PROGMEM char PS2Keymap_US_shift[] ={
        0, PS2_F9, 0, PS2_F5, PS2_F3, PS2_F1, PS2_F2, PS2_F12,
	0, PS2_F10, PS2_F8, PS2_F6, PS2_F4, PS2_TAB, '~', 0,
	0, 0 /*Lalt*/, 0 /*Lshift*/, 0, 0 /*Lctrl*/, 'Q', '!', 0,
	0, 0, 'Z', 'S', 'A', 'W', '@', 0,
	0, 'C', 'X', 'D', 'E', '$', '#', 0,
	0, ' ', 'V', 'F', 'T', 'R', '%', 0,
	0, 'N', 'B', 'H', 'G', 'Y', '^', 0,
	0, 0, 'M', 'J', 'U', '&', '*', 0,
	0, '<', 'K', 'I', 'O', ')', '(', 0,
	0, '>', '?', 'L', ':', 'P', '_', 0,
	0, 0, '"', 0, '{', '+', 0, 0,
	0 /*CapsLock*/, 0 /*Rshift*/, PS2_ENTER /*Enter*/, '}', 0, '|', 0, 0,
	0, 0, 0, 0, 0, 0, PS2_BACKSPACE, 0,
	0, '1', 0, '4', '7', 0, 0, 0,
	'0', '.', '2', '5', '6', '8', PS2_ESC, 0 /*NumLock*/,
	PS2_F11, '+', '3', '-', '*', '9', PS2_SCROLL, 0,
	0, 0, 0, PS2_F7 };


//v
//g
//...
//clk
//data

//白色
#define MOUSE_DATA 2
//绿色
#define MOUSE_CLOCK 3

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);

/**
 * Setup
 */
void setup()
{
  Serial.begin(9600);
  //mouse.initialize();
}

/**
 * Main program loop
 */
void loop()
{
  uint8_t data = (uint8_t)mouse.read_byte();
  if(data==0xF0){
    mouse.read_byte();
  }else{
  Serial.print((data>>7)&1); // Status Byte
  Serial.print((data>>6)&1); // Status Byte
  Serial.print((data>>5)&1); // Status Byte
  Serial.print((data>>4)&1); // Status Byte
  Serial.print((data>>3)&1); // Status Byte
  Serial.print((data>>2)&1); // Status Byte
  Serial.print((data>>1)&1); // Status Byte
  Serial.print((data>>0)&1); // Status Byte
  Serial.print(",");
  Serial.print((int)data);
  Serial.print((char)pgm_read_byte(PS2Keymap_US + data));
  Serial.println();
  }
}
