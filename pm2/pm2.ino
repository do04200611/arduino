#include <LiquidCrystal_I2C.h>
//#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
//#include <WiFiUdp.h>

const char *ssid = "DIT_FREE_WiFi";
const char *password = "";

String url = "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=2671025000";

String reh_content,temp_content;

LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid,password);
    
    while(WiFi.status()!= WL_CONNECTED){
      delay(500);
      Serial.print(".");
    }
    lcd.init();
    lcd.backlight();

       
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){
    WiFiClient client;
    HTTPClient http;

    http.begin(client, url);

    int httpCode = http.GET();

    if(httpCode>0){
      String result = http.getString();
      //Serial.println(result);

      int temp1 = result.indexOf("<temp>");
      int temp2 = result.indexOf("</temp>");

      int reh1 = result.indexOf("<reh>");
      int reh2 = result.indexOf("</reh>");

      if(temp1 > 0){
        temp_content =  result.substring(temp1+6, temp2);
        Serial.print("temp_content:");Serial.print(temp_content);Serial.println("C");
      }

      if(reh1 > 0){
        reh_content =  result.substring(reh1+5, reh2);
        Serial.print("reh_content:");Serial.print(reh_content);Serial.println("%");
      }
      
      lcd.setCursor(0,0);
      lcd.print("temp_content:");
      lcd.print(temp_content);
      lcd.print("C");
      lcd.println();
      
      lcd.setCursor(0,1);
      lcd.print("reh_content:");
      lcd.print(reh_content);
      lcd.print("%");
      lcd.println();
      
    }
    http.end();
  }
  delay(10000);
}
