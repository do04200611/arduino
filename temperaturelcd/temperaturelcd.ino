#include <LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

const char *ssid = "DIT_FREE_WiFi"; //사용할 와이파이 ssid
const char *password = "";          // 와이파이 비밀번호

LiquidCrystal_I2C lcd(0x27, 16,2);
String url = "https://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty?serviceKey=JE4gVDiISk0d4wiqXL3gyVE3aL5fbHWt4G%2BwmQhzuw0c2a%2Fc3%2BaG2u0E4Hm8dPOhv3%2FQO1pTwzrw%2BO%2Btiwt5KA%3D%3D&returnType=xml&numOfRows=100&pageNo=1&sidoName=%EC%84%9C%EC%9A%B8&ver=1.0";


void setup() {
  Serial.begin(115200); //시리얼 통신 115200로 시작하기
  WiFi.begin(ssid, password);   // 와이파이 연결하기

  while(WiFi.status() != WL_CONNECTED) {  //와이파이 연결이 성공할 때까지
    Serial.print(".");  //. 표시 출력
    delay(500);         // 딜레이
  }
  Serial.println("WiFi connected"); //와이파이 연결 성공하면 연결 성공 메시지
  Serial.print("IP : ");Serial.print(WiFi.localIP());   //와이파이 ip 주소 출력하기
  lcd.init();
  lcd.backlight();
}

void loop() {
  if(WiFi.status() == WL_CONNECTED) {
    WiFiClientSecure client;  // 와이파이 클라이언트 객체 생성
    client.setInsecure();
    client.connect(url, 443);
    
    HTTPClient http;    //http 클라이언트 객체 생성
    http.begin(client, url);  //http 요청하려고 url 설정

    int httpCode = http.GET();  //http get 요청 보내기

    if(httpCode>0) {
      String result = http.getString(); //http 내용을 문자열로 바꿔 받기
      Serial.println(result);

      int pm10_1 = result.indexOf("<pm10Value>");
      int pm10_2 = result.indexOf("</pm10Value>"); 
      String pm10 = "<pm10Value>";
 
      String content = result.substring(pm10_1 + pm10.length(), pm10_2); 
      Serial.print(content); 

      lcd.setCursor(0,0);
      lcd.print("pm10: ");
      lcd.setCursor(7,0);
      lcd.print(content);
    }
    http.end();
  }
  delay(6000);
}
