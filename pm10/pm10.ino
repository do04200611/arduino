#include <ESP8266WiFi.h>  // ESP8266용 Wi-Fi 라이브러리
#include <ESP8266HTTPClient.h>  // 다른 웹 서버와 통신하기 위한 라이브러리
#include <WiFiClient.h>

const char *ssid = "DIT_FREE_WiFi";  // 연결할 Wi-Fi 네트워크의 SSID (네트워크 이름)
const char *password = "";  // Wi-Fi 네트워크의 비밀번호

String url = "http://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=2671025000";  // 기상청 RSS 피드의 URL

void setup() {
  Serial.begin(9600);  // 시리얼 통신 초기화 및 보드 속도 설정

  WiFi.begin(ssid, password);  // Wi-Fi 네트워크에 연결을 시도

  while(WiFi.status() != WL_CONNECTED){  // Wi-Fi 연결이 완료될 때까지 대기
    Serial.print(".");
    delay(500);
  }

  Serial.println("WIFI CONNECTED");  // Wi-Fi 연결이 완료되면 메시지 출력
  Serial.print("IP : "); Serial.print(WiFi.localIP());  // 연결된 IP 주소 출력
}

void loop() {
  if(WiFi.status() == WL_CONNECTED){  // Wi-Fi 연결이 유지되고 있는 경우에만 코드 실행
    WiFiClient client;  // Wi-Fi 클라이언트 객체 생성
    HTTPClient http;  // HTTP 클라이언트 객체 생성

    http.begin(client, url);  // HTTP 요청을 위한 준비

    int httpCode = http.GET();  // HTTP GET 요청을 보내고 응답 코드를 받아옴

    if(httpCode > 0) {  // 유효한 응답 코드인 경우에만 처리
      String result = http.getString();  // 서버로부터 받은 응답을 문자열로 저장

      // 온도 정보 추출
      int temp1 = result.indexOf("<temp>");
      int temp2 = result.indexOf("</temp>");
      if(temp1 > 0){
        String temp_content = result.substring(temp1 + 6, temp2);
        Serial.print("temp content : "); Serial.print(temp_content); Serial.println("C");
      }

      // 습도 정보 추출
      int reh1 = result.indexOf("<reh>");
      int reh2 = result.indexOf("</reh>");
      if(reh1 > 0){
        String reh_content = result.substring(reh1 + 5, reh2);
        Serial.print("reh content : "); Serial.print(reh_content); Serial.println("C");
      }
    }

    http.end();  // HTTP 요청 종료
  }

  delay(10000);  // 10초간 대기 후 다음 루프 실행
}
