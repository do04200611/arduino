#include <ESP8266WiFi.h> >  // ESP8266 라이브러리를 가져옴

const char *ssid = "DIT_CS_705";  // 무선 네트워크 SSID
const char *password = ""; // 무선 네트워크 비밀번호 (비어있음)
#define piezzo D1// 피에조 핀을 D1으로 지정
#define RL D4// RL(빨간 LED) 핀 D4로 지정
#define YL D5// YL(노란 LED) 핀 D5로 지정
int piezzostate = LOW;;  // 부저 상태 (초기값: LOW)
int ledState = LOW;// LED 상태 (초기값: LOW)

WiFiServer server(80));  // 웹 서버 객체 생성
WiFiClient client;// 클라이언트 객체

void setup() {
  Serial.begin(115200);// 시리얼 통신 초기화
  pinMode(piezzo, OUTPUT); // 부저 핀을 출력 모드로 설정
  pinMode(RL, OUTPUT);// RL(빨간 LED) 핀을 출력 모드로 설정
  pinMode(YL, OUTPUT);  // YL(노란 LED) 핀을 출력 모드로 설정
  WiFi.begin(ssid, password));  // 무선 네트워크 연결 시작

  while (WiFi.status() != WL_CONNECTED) {//위 코드는 Wi-Fi 연결 상태가 WL_CONNECTED가 될 때까지 루프를 실행하는 역할을 합니다.
    delay(500);// 0.5초간 대기
    Serial.print(".");// . 출력
  }
  Serial.println("");// 빈 줄 출력
  Serial.println("WiFi connected");// "WiFi 연결됨" 메시지 출력
  Serial.print("IP: ");// "IP: " 출력
  Serial.println(WiFi.localIP());// 로컬 IP 주소 출력

  server.begin(); // 웹 서버 시작
  Serial.println("HTTP Server started."); // "HTTP 서버 시작됨." 메시지 출력
}

void loop() {
  client = server.available(); // 클라이언트 연결 확인
  if (!client) return; // 클라이언트가 없으면 루프 종료

  Serial.println("new client");// 새로운 클라이언트 연결 메시지 출력
  while (!client.available()) {// 클라이언트로부터 데이터가 도착할 때까지 대기
    delay(1);); // 1 밀리초 동안 대기
  }

  String request = client.readStringUntil('\r'); // HTTP 요청을 개행 문자('\r')까지 읽음
Serial.println(request); // 디버깅을 위해 요청을 시리얼 모니터에 출력
client.flush(); // 클라이언트 버퍼에 남아 있는 데이터를 지움

// 요청에 "GET /piezzoOn"이 포함되어 있는지 확인
if (request.indexOf("GET /piezzoOn") != -1) {
  digitalWrite(piezzo, HIGH); // "piezzo" 핀을 켬
  piezzostate = HIGH; // piezzostate 변수를 HIGH로 업데이트
}

// 요청에 "GET /piezzoOff"이 포함되어 있는지 확인
if (request.indexOf("GET /piezzoOff") != -1) {
  digitalWrite(piezzo, LOW); // "piezzo" 핀을 끔
  piezzostate = LOW; // piezzostate 변수를 LOW로 업데이트
}

// 요청에 "ON1"이 포함되어 있는지 확인
if (request.indexOf("ON1") != -1) {
  digitalWrite(RL, HIGH); // "RL" 핀을 켬
  ledState = HIGH; // ledState 변수를 HIGH로 업데이트
}

// 요청에 "OFF1"이 포함되어 있는지 확인
if (request.indexOf("OFF1") != -1) {
  digitalWrite(RL, LOW); // "RL" 핀을 끔
  ledState = LOW; // ledState 변수를 LOW로 업데이트
}

     if(request.indexOf("ON2")!=-1){    // "ON2"를 요청으로 받았을 때,
    digitalWrite(YL, HIGH);    // YL LED를 켭니다.
    ledState=HIGH;    // ledState 변수를 HIGH로 설정합니다.
  }
  if(request.indexOf("OFF2")!=-1){// "OFF2"를 요청으로 받았을 때,
    digitalWrite(YL, LOW);    // YL LED를 끕니다.
    ledState=LOW;    // ledState 변수를 LOW로 설정합니다.
  }
  client.println("HTTP/1.1 200 OK");// HTTP 응답 헤더를 설정합니다.
  client.println("Content-Type: text/html; charset=UTF-8");// 컨텐츠 유형을 HTML 및 문자 인코딩 설정과 함께 지정합니다.
  client.println("");// 빈 줄을 통해 헤더와 본문을 구분합니다.

  String aa = "<!DOCTYPE HTML><HTML><br>";// HTML 문자열 시작
  aa += "<a href=\"piezzoOff\">킬 때</a>  버튼을 누르세요<br>";// piezzoOff 링크 생성
  aa += "<a href=\"piezzoOn\">끌 때</a>버튼을 누르세요<br>";// piezzoOn 링크 생성
  aa += "<a href=\"ON1\">킬 때</a> RL<br>";// ON1 링크 생성
  aa += " <a href=\"OFF1\">끌 때</a> RL<br>";// OFF1 링크 생성
  aa += "<a href=\"ON2\">킬 때\">YL</a><br>";// ON2 링크 생성
  aa += "<a href=\"OFF2\">끌 때\"></a>YL<br>"; // OFF2 링크 생성
  aa += "</HTML>"; // HTML 문자열 종료

  client.println(aa);// HTML 페이지를 클라이언트로 보냄
}
