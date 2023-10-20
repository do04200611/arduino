#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char* ssid = "DIT_cs_703";
const char* password = "";

WiFiServer server(80);
WiFiClient client;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connecting to");
  Serial.println(ssid);
  Serial.print("IP address");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("HTTP Server started");
}

void loop() {
  // put your main code here, to run repeatedly:
  client = server.available();

  if(!client)return;

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");

  String content ="<!DOCTYPE HTML><HTML><h2>Hi, Web Server!<h2></HTML>";
  client.println(content);
}

//만물수리철물
