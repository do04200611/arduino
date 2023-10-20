#include <LiquidCrystal_I2C.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

LiquidCrystal_I2C lcd(0x27,16,2);

//와이파이 이름 및 비번 정의 및 초기화
const char *ssid = "DIT_FREE_WiFi";
const char *password = "";

//연결할 기상청 공공api 정의 및 초기화
String url = "https://apis.data.go.kr/B552584/ArpltnInforInqireSvc/getCtprvnRltmMesureDnsty?serviceKey=JE4gVDiISk0d4wiqXL3gyVE3aL5fbHWt4G%2BwmQhzuw0c2a%2Fc3%2BaG2u0E4Hm8dPOhv3%2FQO1pTwzrw%2BO%2Btiwt5KA%3D%3D&returnType=xml&numOfRows=100&pageNo=1&sidoName=%EC%84%9C%EC%9A%B8&ver=1.0";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

    //와이파이 연결될 때가지의 동작
    while(WiFi.status() != WL_CONNECTED){
    delay(500);
  }

    //와이파이 연결 성공 시
    Serial.println("Wifi connected");
    Serial.print("IP : "); Serial.print(WiFi.localIP());
    Serial.println();

    //lcd 모니터 동작
    lcd.init(); // lcd init                                                                                            
    lcd.backlight(); // lcd back light on
}

void loop() {
    //와이파이가 연결되있는지 확인
    if(WiFi.status() == WL_CONNECTED){

      //클라이언트 객체 생성
      WiFiClientSecure client;
      client.setInsecure();
      client.connect(url, 443);
      
      HTTPClient http;
      
      //HTTP 요청을 보낼 대상 서버와 URL을 설정
      http.begin(client, url);

      //HTTP GET 요청을 보내고, 서버로부터의 응답 코드를 받아옴
      int httpCode = http.GET();

      //응답 코드가 0보다 크다면 아래 코드를 실행
      if(httpCode>0){

        //서버에서 받아온 응답을 문자열로 저장
        String result = http.getString();
        Serial.println(result);

      }

      http.end();
    }
    delay(500);
}
