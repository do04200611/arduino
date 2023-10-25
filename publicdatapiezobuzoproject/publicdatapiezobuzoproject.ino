#include <ESP8266WiFi.h>

const char *ssid = "DIT_FREE_WiFi";
const char *password = "";
#define piezzo D1
int piezzostate = LOW;

WiFiServer server(80);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  pinMode(piezzo, OUTPUT);
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

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html; charset=UTF-8");
  client.println("");

  String response = "<!DOCTYPE HTML><HTML><br>";
  response += "Click <a href=\"/piezzoOff\">here</a> 버튼을 누르세요<br>";
  response += "Click <a href=\"/piezzoOn\">here</a> 버튼을 누르세요<br>";
  response += "</HTML>";

  client.println(response);
}
