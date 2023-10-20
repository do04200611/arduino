#include <LiquidCrystal_I2C.h>


#include <ESP8266WiFi.h>
const char *ssid = "DIT_CS_705";
const char *password = "";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  int numOfWifi = WiFi.scanNetworks();
 
  
  for(int i=0; i<numOfWifi; i++){
    Serial.print("Wifi 갯수: "); Serial.println(numOfWifi);
    Serial.print("WiFi name");
    Serial.print(WiFi.SSID(i));
    Serial.print("Signal Strength:");

    //WiFi.Rssi()는 현재 공유기의 무서강도를 dBM 단위로 돌려준다.
    //바로 앞이면 -45BM 수준에 가까운 -dBM값을 준다.
    Serial.println(WiFi.RSSI(i));
    Serial.print("============================ ");
  delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
