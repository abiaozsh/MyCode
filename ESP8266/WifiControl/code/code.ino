#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

/*
static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t D9   = 3;
static const uint8_t D10  = 1;

 */
#define powerD D1
#define resetD D2

#define volt5D D4
#define hddD D0

const char* t_p = "text/plain";
const char* ssid = "zsh2";
const char* password = "92066200";

ESP8266WebServer server(80);

void handleRoot() {
  server.send(200, "text/html", "<html></html>");
}

void handlePage() {
  String ret = "";
  ret+="<html>";
  ret+="return:<input id=\"J_read\" type=\"text\" style=\"width:250px\" value=\"\"><br/>";
  ret+="<input id=\"J_P\" type=\"button\" style=\"height:70px\" value=\"_____PowerButton_____\"><br/><br/><br/>";
  ret+="<input id=\"J_S\" type=\"button\" style=\"height:100px\" value=\"_____Status_______\"><br/><br/><br/><br/><br/><br/>";
  ret+="<input id=\"J_LP\" type=\"button\" style=\"height:30px\" value=\"LPB\"><br/><br/><br/>";
  ret+="<input id=\"J_R\" type=\"button\" style=\"height:30px\" value=\"RST\"><br/><br/><br/>";
  ret+="<script src=\"base.js\" ></script>";
  ret+="<script>";
  ret+="JQ(\"J_P\").onclick=function(){";
  ret+="_ajaxGet(\"/power\",\"\",function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_LP\").onclick=function(){";
  ret+="_ajaxGet(\"/longpower\",\"\",function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_R\").onclick=function(){";
  ret+="_ajaxGet(\"/reset\",\"\",function(data){JQ(\"J_read\").value = data;});";
  ret+="};";
  ret+="JQ(\"J_S\").onclick=function(){";
  ret+="_ajaxGet(\"/status\",\"\",function(data){JQ(\"J_read\").value = data;});";
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

volatile unsigned long currentMillisTime = 0;
volatile int V1 = 0;
volatile int V2 = 0;

volatile int _V1 = 0;
volatile int _V2 = 0;

volatile int isonP = 0;
volatile unsigned long currentMillisP = 0;

void _power() {
  digitalWrite(powerD, 1);
  isonP = 1;
  currentMillisP = millis();
  String ret = "ok";
  server.send(200, t_p, ret);
}

volatile int isonPL = 0;
volatile unsigned long currentMillisPL = 0;

void _longpower() {
  digitalWrite(powerD, 1);
  isonPL = 1;
  currentMillisPL = millis();
  String ret = "ok";
  server.send(200, t_p, ret);
}

volatile int isonR = 0;
volatile unsigned long currentMillisR = 0;

void _reset() {
  digitalWrite(resetD, 1);
  isonR = 1;
  currentMillisR = millis();
  String ret = "ok";
  server.send(200, t_p, ret);
}

void _status() {
  int v = digitalRead(volt5D);
  String ret = "ok ";
  ret += v == 0 ? "PWR on " : "PWR off ";//0 pwr on         1 pwr off
  float vv1 = _V1;
  float vv2 = _V2;
  vv1 = (vv1*100/(vv1+vv2));
  ret += "HDD :";
  ret += vv1;//  _V1; // hdd on     _V2;//hdd off

  server.send(200, t_p, ret);
}

void handleNotFound() {
  server.send(404, t_p, "File Not Found");
}

void setup(void) {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("poweron");

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
  server.on("/main", handlePage);
  server.on("/base.js", basejs);
  
  
  server.on("/power", _power);
  server.on("/longpower", _longpower);
  server.on("/reset", _reset);
  server.on("/status", _status);

  server.onNotFound(handleNotFound);

  server.begin();
  
  digitalWrite(powerD, 0);
  digitalWrite(resetD, 0);
  pinMode(powerD, OUTPUT);
  pinMode(resetD, OUTPUT);

  Serial.println("started");
}

void loop(void) {
  server.handleClient();
  unsigned long mills = millis();
  
  int v = digitalRead(hddD);
  if(v){
    V1++;
  }else{
    V2++;
  }
  
  if(mills - currentMillisTime > 1000){
    currentMillisTime = mills;
    _V1 = V1;
    _V2 = V2;
    V1 = 0;
    V2 = 0;
  }
  
  if(isonP){
    if(mills - currentMillisP > 200){
      digitalWrite(powerD, 0);
      isonP = 0;
    }
  }
  
   if(isonPL){
    if(mills - currentMillisPL > 5000){
      digitalWrite(powerD, 0);
      isonPL = 0;
    }
  }

  if(isonR){
    if(mills - currentMillisR > 200){
      digitalWrite(resetD, 0);
      isonR = 0;
    }
  }
  
  
}
