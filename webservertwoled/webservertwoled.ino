#include <ESP8266WiFi.h>

const char *ssid = "DIT_CS_705";
const char *password = "";
#define RED_LED D1
#define YEL_LED D2
int ledState = LOW;

WiFiServer server(80);
WiFiClient client;


void setup() {
  Serial.begin(115200);
  pinMode(RED_LED, OUTPUT);
  pinMode(YEL_LED, OUTPUT);
  WiFi.begin(ssid,password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP :"); Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("HTTP Server started.");

}

void loop() {
  client = server.available();
  if(!client) return;

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if(request.indexOf("LEDON1")!=-1){
    digitalWrite(RED_LED, HIGH);
    ledState=HIGH;
  }
   if(request.indexOf("LEDON2")!=-1){
    digitalWrite(YEL_LED, HIGH);
    ledState=HIGH;
  }
  if(request.indexOf("LEDOFF1")!=-1){
    digitalWrite(RED_LED, LOW);
    ledState=LOW;
  }
  if(request.indexOf("LEDOFF2")!=-1){
    digitalWrite(YEL_LED, LOW);
    ledState=LOW;
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type text/html");
  client.println("");

  String aa = "<!DOCTYPE HTML><HTML><br>";
    aa += "Click <a href=\"LEDON1\">here</a> RED LED ON<br>";
    aa += "Click <a href=\"LEDOFF1\">here</a> RED LED OFF<br>";
    aa += "Click <a href=\"LEDON2\">here</a> YEL LED ON<br>";
    aa += "Click <a href=\"LEDOFF2\">here</a> YEL LED OFF<br>";
    aa += "</HTML>";


  client.println(aa);

}
