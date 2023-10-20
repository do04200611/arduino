#include <ESP8266WiFi.h>
#include <DHT.h>

//와이파이 이름 및 비번 정의 및 초기화
const char *ssid = "DIT_CS_705";
const char *password = "";

WiFiServer server(80);
WiFiClient client;
DHT dht(D2, DHT11);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Connected");
  Serial.print("IP:"); Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("HTTP Server started.");
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  client = server.available();

  if(!client) return;

  Serial.println("new client");
  while(!client.available()){
    delay(1);    
  }
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type:text/html");
  client.println("");

  String content = "<!DOCTYPE HTML><HTML><h2>[Weather Station]</h2>";
    content += "<h3 style=\"background-color:pink\">Temperature : ";
    content += temp;
    content += "C</h3>" ;
    content += "<h3 style=\"background-color:lightblue\">Humidity : ";
    content += humid;
    content += "%</h3><br>" ;
  client.println(content);
}
