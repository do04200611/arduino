#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <LiquidCrystal_I2C.h>

const char *ssid ="DIT_CS_705";
const char *password ="";
String url ="http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=2671025000";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while(WiFi.status()!= WL_CONNECTED){
    Serial.print(".");
    delay(500);
  }
  Serial.print("WiFi connected");
  Serial.print("IP");Serial.print(WiFi.localIP());
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  if(WiFi.status()== WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;
    
    http.begin(client, url);

    int httpCode = http.GET();

    if(httpCode>0){
      String result = http.getString();
      Serial.println(result);
      
    }
    http.end();
  }
  delay(10000);
}
