#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
/*

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

const char* ssid = "zsh2";
const char* password = "92066200";

ESP8266WebServer server(80);

const int led = LED_BUILTIN;

void handleRoot() {
  digitalWrite(led, 1);
  server.send(200, "text/plain", "hello from esp8266!");
  //digitalWrite(led, 0);
}

void handleNotFound(){
   digitalWrite(led, 0);
 //digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET)?"GET":"POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i<server.args(); i++){
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void setup(void){
  pinMode(D0, OUTPUT);
pinMode(D1, OUTPUT);
pinMode(D2, OUTPUT);
pinMode(D3, OUTPUT);
pinMode(D4, OUTPUT);
pinMode(D5, OUTPUT);
pinMode(D6, OUTPUT);
pinMode(D7, OUTPUT);
pinMode(D8, OUTPUT);

  
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  
  server.on("/0a", [](){digitalWrite(D0, 0);server.send(200, "text/plain", "ok");});
  server.on("/0b", [](){digitalWrite(D0, 1);server.send(200, "text/plain", "ok");});
  server.on("/1a", [](){digitalWrite(D1, 0);server.send(200, "text/plain", "ok");});
  server.on("/1b", [](){digitalWrite(D1, 1);server.send(200, "text/plain", "ok");});
  server.on("/2a", [](){digitalWrite(D2, 0);server.send(200, "text/plain", "ok");});
  server.on("/2b", [](){digitalWrite(D2, 1);server.send(200, "text/plain", "ok");});
  server.on("/3a", [](){digitalWrite(D3, 0);server.send(200, "text/plain", "ok");});
  server.on("/3b", [](){digitalWrite(D3, 1);server.send(200, "text/plain", "ok");});
  server.on("/4a", [](){digitalWrite(D4, 0);server.send(200, "text/plain", "ok");});
  server.on("/4b", [](){digitalWrite(D4, 1);server.send(200, "text/plain", "ok");});
  server.on("/5a", [](){digitalWrite(D5, 0);server.send(200, "text/plain", "ok");});
  server.on("/5b", [](){digitalWrite(D5, 1);server.send(200, "text/plain", "ok");});
  server.on("/6a", [](){digitalWrite(D6, 0);server.send(200, "text/plain", "ok");});
  server.on("/6b", [](){digitalWrite(D6, 1);server.send(200, "text/plain", "ok");});
  server.on("/7a", [](){digitalWrite(D7, 0);server.send(200, "text/plain", "ok");});
  server.on("/7b", [](){digitalWrite(D7, 1);server.send(200, "text/plain", "ok");});
  server.on("/8a", [](){digitalWrite(D8, 0);server.send(200, "text/plain", "ok");});
  server.on("/8b", [](){digitalWrite(D8, 1);server.send(200, "text/plain", "ok");});

  server.on("/inline", [](){
    digitalWrite(led, 0);
    server.send(200, "text/plain", "this works as well");
  });

  server.onNotFound(handleNotFound);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();
}
