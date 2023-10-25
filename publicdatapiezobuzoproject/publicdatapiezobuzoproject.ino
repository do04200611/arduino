#include <ESP8266WiFi.h>

const char *ssid = "DIT_CS_705";
const char *password = "";
#define piezzo D1
#define RL D4
#define YL D5
int piezzostate = LOW;
int ledState = LOW;

WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(piezzo, OUTPUT);
  pinMode(RL, OUTPUT);
  pinMode(YL, OUTPUT);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
  Serial.println("HTTP Server started.");
}

void loop() {
  client = server.available();
  if (!client) return;
=
  Serial.println("new client");
  while (!client.available()) {
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if (request.indexOf("GET /piezzoOn") != -1) {
    digitalWrite(piezzo, HIGH);
    piezzostate = HIGH;
  }
  if (request.indexOf("GET /piezzoOff") != -1) {
    digitalWrite(piezzo, LOW);
    piezzostate = LOW;
  }
    if(request.indexOf("ON1")!=-1){
    digitalWrite(RL, HIGH);
    ledState=HIGH;
  }
  if(request.indexOf("OFF1")!=-1){
    digitalWrite(RL, LOW);
    ledState=LOW;
  }
     if(request.indexOf("ON2")!=-1){
    digitalWrite(YL, HIGH);
    ledState=HIGH;
  }
  if(request.indexOf("OFF2")!=-1){
    digitalWrite(YL, LOW);
    ledState=LOW;
  }
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("");

  String aa = "<!DOCTYPE HTML><HTML><br>";
  aa += "<a href=\"piezzoOff\">킬 때</a>  버튼을 누르세요<br>";
  aa += "<a href=\"piezzoOn\">끌 때</a>버튼을 누르세요<br>";
  aa += "<a href=\"ON1\">킬 때</a> RL<br>";
  aa += " <a href=\"OFF1\">끌 때</a> RL<br>";
  aa += "<a href=\"ON2\">킬 때\">YL</a><br>";
  aa += "<a href=\"OFF2\">끌 때\"></a>YL<br>";
  aa += "</HTML>";

  client.println(aa);
}
