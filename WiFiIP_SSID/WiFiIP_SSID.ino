#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>

#include <ESP8266WiFi.h>
//
const char *ssid = "DIT_FREE_WiFi";
const char *password = "";

//const char *ssid = "DIT_CS_705";
//const char *password = "";


//const char *ssid = "iptime";
//const char *password = "Ca19601980";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(500);
    Serial.print(".");
  }
   
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println();
   Serial.print("Wifi connected");
   Serial.print("IP:");
   Serial.print(WiFi.localIP());
}
