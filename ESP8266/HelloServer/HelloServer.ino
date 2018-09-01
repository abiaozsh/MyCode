#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

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
D0-led-vcc


  static const uint8_t SDA = 4;
  static const uint8_t SCL = 5;

  static const uint8_t LED_BUILTIN = 2;//D4
  static const uint8_t BUILTIN_LED = 2;//D4

  static const uint8_t D3   = 0;
  static const uint8_t TX   = 1;
  static const uint8_t D4   = 2;
  static const uint8_t RX   = 3;
  static const uint8_t D2   = 4;
  static const uint8_t D1   = 5;
  static const uint8_t D6   = 12;
  static const uint8_t D7   = 13;
  static const uint8_t D5   = 14;
  static const uint8_t D8   = 15;
  static const uint8_t D0   = 16;

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

ESP8266WebServer server(80);

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

void handleRoot() {
	String ret = "";
  ret+="<html>";
  ret+="return:<input id=\"J_read\" type=\"text\" value=\"\"><br/>";
  ret+="<input id=\"J_pin\" type=\"text\" value=\"D0\"><br/>";
  ret+="<input id=\"J_freq\" type=\"text\" value=\"020000\">";
  ret+="<input id=\"J_range\" type=\"text\" value=\"001023\">";
  ret+="<input id=\"J_val\" type=\"text\" value=\"000012\"><br/>";
  ret+="<input id=\"J_in\" type=\"button\" value=\"__IN__\">";
  ret+="<input id=\"J_out\" type=\"button\" value=\"__OUT__\">";
  ret+="<input id=\"J_inpu\" type=\"button\" value=\"__INPU__\">";
  ret+="<input id=\"J_outod\" type=\"button\" value=\"__OUOD__\"><br/>";
  ret+="<input id=\"J_hi\" type=\"button\" value=\"__HI__\">";
  ret+="<input id=\"J_lo\" type=\"button\" value=\"__LO__\"><br/>";
  ret+="<input id=\"J_dread\" type=\"button\" value=\"_D_Read__\">";
  ret+="<input id=\"J_aread\" type=\"button\" value=\"_A_Read__\"><br/>";
  ret+="<input id=\"J_aWrite\" type=\"button\" value=\"_A_Write__\"><br/>";
  ret+="<script src=\"base.js\" ></script>";
  ret+="<script>";
  ret+="JQ(\"J_in\").onclick=function(){pinmode(\"IN\");};";
  ret+="JQ(\"J_out\").onclick=function(){pinmode(\"OU\");};";
  ret+="JQ(\"J_inpu\").onclick=function(){pinmode(\"INPU\");};";
  ret+="JQ(\"J_outod\").onclick=function(){pinmode(\"OUOD\");};";
  ret+="var pinmode=function(mode){";
  ret+="_ajaxGet(\"/pinMode\",\"pin=\"+JQ(\"J_pin\").value+\"&mode=\"+mode,function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_hi\").onclick=function(){digitalWrite(\"1\")};";
  ret+="JQ(\"J_lo\").onclick=function(){digitalWrite(\"0\")};";
  ret+="var digitalWrite=function(val){";
  ret+="_ajaxGet(\"/digitalWrite\",\"pin=\"+JQ(\"J_pin\").value+\"&val=\"+val,function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_dread\").onclick=function(){";
  ret+="_ajaxGet(\"/digitalRead\",\"pin=\"+JQ(\"J_pin\").value,function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_aread\").onclick=function(){";
  ret+="_ajaxGet(\"/analogRead\",\"\",function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_aWrite\").onclick=function(){";
  ret+="  _ajaxGet(\"/analogWrite\",";
  ret+="\"pin=\"+JQ(\"J_pin\").value+";
  ret+="\"&freq=\"+JQ(\"J_freq\").value+";
  ret+="\"&range=\"+JQ(\"J_range\").value+";
  ret+="\"&val=\"+JQ(\"J_val\").value";
  ret+=",function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="</script>";
  ret+="end</html>";
  server.send(200, "text/html", ret);
}

void basejs() {
  String ret = "";
  ret +="function getxhr() {";
  ret +="var a=false;";
  ret +="try{a=new ActiveXObject(\"Msxml2.XMLHTTP\");}catch (e){";
  ret +="try{a=new ActiveXObject(\"Microsoft.XMLHTTP\");} catch (e2) {";
  ret +="a = false;}}";
  ret +="if (!a && typeof XMLHttpRequest != 'undefined') {a = new XMLHttpRequest();}";
  ret +="return a;}";
  ret +="var xhr = getxhr();";
  ret +="function _ajaxGet(url,cont,cb) {";
  ret +="xhr.open(\"POST\",url,true);";
  ret +="xhr.setRequestHeader(\"Content-Type\",\"application/x-www-form-urlencoded\");";
  ret +="xhr.setRequestHeader(\"If-Modified-Since\", \"0\");";
  ret +="xhr.onreadystatechange = wacthing;";
  ret +="function wacthing() {";
  ret +="if(xhr.readyState==4) {";
  ret +="if(xhr.status==200) {";
  ret +="var res = xhr.responseText;";
  ret +="cb(res);}}}";
  ret +="xhr.send(cont);}";
  ret +="var JQ = function(id){return document.getElementById(id);}";

  server.send(200, "text/html", ret);
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

volatile int ison = 0;
unsigned long currentMillis = 0;

void test() {
  int pin = D0;
  int val = 1;

  String ret = "ok";
  server.send(200, t_p, ret);
  
  digitalWrite(D0, 1);
  ison = 1;
  currentMillis = millis();
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

int _parseInt(String v){
  int ret = ((int)(v.charAt(5)-'0')) + 
  ((int)(v.charAt(4)-'0'))*10 + 
  ((int)(v.charAt(3)-'0'))*100 + 
  ((int)(v.charAt(2)-'0'))*1000 + 
  ((int)(v.charAt(1)-'0'))*10000 + 
  ((int)(v.charAt(0)-'0'))*100000;
  return ret;
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
  
  String ret = "ok pin:";
  ret += pin;
  ret += ",freq:";
  ret += freq;
  ret += ",range:";
  ret += range;
  ret += ",val:";
  ret += val;

  analogWriteFreq(freq);//20000
  analogWriteRange(range);//1023
  analogWrite(pin,val);

  server.send(200, t_p, ret);
}

void handleNotFound() {
  server.send(404, t_p, "File Not Found");
}

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("start");

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
  server.on("/test", test);

//  server.on("/inline", []() {
//    digitalWrite(led, 0);
//    server.send(200, t_p, "this works as well");
//  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("started");
}

void loop(void) {
  server.handleClient();
  if(ison){
    unsigned long mills = millis();  
    if(mills - currentMillis > 2000){
      digitalWrite(D0, 0);
      ison = 0;
    }
  }
}
