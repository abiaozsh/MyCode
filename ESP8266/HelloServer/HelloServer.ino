#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

/*
  analogWriteFreq(20000);
  analogWriteRange(1023);
  analogWrite(D4,126);
  return system_adc_read();
  digitalRead(pin)
  Line 71:   pinMode(twi_sda, INPUT_PULLUP);
  Line 72:   pinMode(twi_scl, INPUT_PULLUP);
  digitalWrite(D0, 0);
  pinMode(twi_sda, INPUT);

  //GPIO FUNCTIONS
  #define INPUT             0x00
  #define INPUT_PULLUP      0x02
  #define INPUT_PULLDOWN_16 0x04 // PULLDOWN only possible for pin16
  #define OUTPUT            0x01
  #define OUTPUT_OPEN_DRAIN 0x03


  VIN GND RST EN 3V3 GND CLK SDO CMD SD1 SD2 SD3 NC ADC A0
  3V3 GND TX  RX D8  D7  D6  D5  GND 3V3  D4  D3 D2 D1  D0


  static const uint8_t SDA = 4;
  static const uint8_t SCL = 5;

  static const uint8_t LED_BUILTIN = 2;//D4
  static const uint8_t BUILTIN_LED = 2;//D4

  static const uint8_t D0   = 16;
  static const uint8_t D1   = 5;
  static const uint8_t D2   = 4;
  static const uint8_t D3   = 0;
  static const uint8_t D4   = 2;
  static const uint8_t D5   = 14;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D8   = 15;
  static const uint8_t RX   = 3;
  static const uint8_t TX   = 1;
*/
const char* t_p = "text/plain";
const char* ssid = "zsh2";
const char* password = "92066200";

int getPin(String v) {
  if (v == "D0")return D0;
  if (v == "D1")return D1;
  if (v == "D2")return D2;
  if (v == "D3")return D3;
  if (v == "D4")return D4;
  if (v == "D5")return D5;
  if (v == "D6")return D6;
  if (v == "D7")return D7;
  if (v == "D8")return D8;
  if (v == "TX")return TX;
  if (v == "RX")return RX;
  if (v == "0")return 0;
  if (v == "1")return 1;
  if (v == "2")return 2;
  if (v == "3")return 3;
  if (v == "4")return 4;
  if (v == "5")return 5;
  if (v == "6")return 6;
  if (v == "7")return 7;
  if (v == "8")return 8;
  if (v == "9")return 9;
  if (v == "10")return 10;
  if (v == "11")return 11;
  if (v == "12")return 12;
  if (v == "13")return 13;
  if (v == "14")return 14;
  if (v == "15")return 15;
  if (v == "16")return 16;
}
int getVal(String v) {
  if (v == "1")return 1;
  if (v == "0")return 0;
}
int getMode(String v) {
  if (v == "IN")return INPUT;
  if (v == "INPU")return INPUT_PULLUP;
  if (v == "OU")return OUTPUT;
  if (v == "OUOD")return OUTPUT_OPEN_DRAIN;
}

int _parseInt(String v){
  int ret = (v.charAt(0)-'0') + 
  (v.charAt(1)-'0')*10 + 
  (v.charAt(2)-'0')*100 + 
  (v.charAt(3)-'0')*1000 + 
  (v.charAt(4)-'0')*10000 + 
  (v.charAt(5)-'0')*100000;
}
ESP8266WebServer server(80);

const int led = LED_BUILTIN;

void handleRoot() {
  server.send(200, "text/html", "\
  <html>\
  <input id=\"btnaaa\" type=\"button\" value=\"val1\">\
  <input id=\"btnbbb\" type=\"button\" value=\"val0\">\
  <input id=\"btnccc\" type=\"button\" value=\"out\">\
  <script src=\"base.js\" ></script>\
  <script>\
  document.getElementById(\"btnaaa\").onclick=function(){\
    _ajaxGet(\"/digitalWrite\",\"pin=D2&val=1\",function(data){alert(data)});\
  };\
  document.getElementById(\"btnbbb\").onclick=function(){\
    _ajaxGet(\"/digitalWrite\",\"pin=D2&val=0\",function(data){alert(data)});\
  };\
  document.getElementById(\"btnccc\").onclick=function(){\
    _ajaxGet(\"/pinMode\",\"pin=D2&mode=OU\",function(data){alert(data)});\
  };\
  </script>\
  </html>\
\
");
}

void basejs() {
  server.send(200, "text/html", "\
function getxhr() {\
var a=false;\
try{a=new ActiveXObject(\"Msxml2.XMLHTTP\");}catch (e){\
try{a=new ActiveXObject(\"Microsoft.XMLHTTP\");} catch (e2) {\
a = false;}}\
if (!a && typeof XMLHttpRequest != 'undefined') {a = new XMLHttpRequest();}\
return a;}\
var xhr = getxhr();\
function _ajaxGet(url,cont,cb) {\
xhr.open(\"POST\",url,true);\
xhr.setRequestHeader(\"Content-Type\",\"application/x-www-form-urlencoded\");\
xhr.setRequestHeader(\"If-Modified-Since\", \"0\");\
xhr.onreadystatechange = wacthing;\
function wacthing() {\
if(xhr.readyState==4) {\
if(xhr.status==200) {\
var res = xhr.responseText;\
cb(res);}}}\
xhr.send(cont);}");
}

void my_pinMode() {
  int pin;
  int mode;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "pin") {
      pin = getPin(server.arg(i));
    }
    if (server.argName(i) == "mode") {
      mode = getMode(server.arg(i));
    }
    //message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  String ret = "ok pin:";
  ret += pin;
  ret += ",mode:";
  ret += mode;
  server.send(200, t_p, ret);
  pinMode(pin, mode);
}
void my_digitalWrite() {
  int pin;
  int val;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "pin") {
      pin = getPin(server.arg(i));
    }
    if (server.argName(i) == "val") {
      val = getVal(server.arg(i));
    }
  }
  //message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  String ret = "ok pin:";
  ret += pin;
  ret += ",val:";
  ret += val;
  server.send(200, t_p, ret);
  digitalWrite(pin, val);
}
void my_analogRead() {
  int analog = analogRead(0);
  String ret = "ok analogVal:";
  ret += analog;
  server.send(200, t_p, ret);
}

void my_digitalRead() {
  int pin;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "pin") {
      pin = getPin(server.arg(i));
    }
  }
  int val = digitalRead(pin);
  String ret = "ok digitalVal:";
  ret += val;
  server.send(200, t_p, ret);
}

void my_analogWrite() {
  int pin;
  int freq;
  int range;
  int val;
  for (uint8_t i = 0; i < server.args(); i++) {
    if (server.argName(i) == "pin") {
      pin = getPin(server.arg(i));
    }
    if (server.argName(i) == "freq") {
      freq = _parseInt(server.arg(i));
    }
    if (server.argName(i) == "range") {
      range = _parseInt(server.arg(i));
    }
    if (server.argName(i) == "val") {
      val = _parseInt(server.arg(i));
    }
  }
  
  analogWriteFreq(freq);//20000
  analogWriteRange(range);//1023
  analogWrite(pin,val);

  String ret = "ok analogVal:";
  ret += val;
  server.send(200, t_p, ret);
}

void handleNotFound() {
  server.send(404, t_p, "File Not Found");
}

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("To:");
  Serial.println(ssid);
  Serial.print("IP:");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/base.js", basejs);
  server.on("/pinMode", my_pinMode);
  server.on("/digitalWrite", my_digitalWrite);
  server.on("/digitalRead", my_digitalRead);
  server.on("/analogRead", my_analogRead);
  server.on("/analogWrite", my_analogWrite);

  server.on("/inline", []() {
    digitalWrite(led, 0);
    server.send(200, t_p, "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("started");
}

void loop(void) {
  server.handleClient();
}
