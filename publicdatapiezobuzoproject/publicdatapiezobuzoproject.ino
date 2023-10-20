#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "Your_SSID";        // 여기에 Wi-Fi 네트워크 이름 (SSID)을 입력하세요
const char* password = "Your_Password";  // 여기에 Wi-Fi 암호를 입력하세요

ESP8266WebServer server(80);

int buzzerPin = D1;  // 피에조 부저를 연결한 핀 번호

bool soundPlaying = false;

void setup() {
  Serial.begin(115200);

  // Wi-Fi 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  server.on("/", HTTP_GET, handleRoot);
  server.begin();
}

void loop() {
  server.handleClient();
  if (soundPlaying) {
    // 여기에 소리 재생 중에 수행할 작업을 추가할 수 있습니다.
  }
}

void handleRoot() {
  String html = "<html><body>";
  html += "<button onclick='playSound()'>소리 재생</button>";
  html += "<button onclick='stopSound()'>소리 정지</button>";
  html += "<script>function playSound() { fetch('/play'); } function stopSound() { fetch('/stop'); }</script>";
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void playSound() {
  tone(buzzerPin, 1000);  // 1000 Hz 주파수로 소리를 내도록 설정
  soundPlaying = true;
}

void stopSound() {
  noTone(buzzerPin);  // 소리 정지
  soundPlaying = false;
}
