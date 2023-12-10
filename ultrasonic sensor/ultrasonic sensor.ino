#include <UniversalTelegramBot.h> // UniversalTelegramBot.h의 라이브러리를 포함하는 코드
#include <ESP8266WiFi.h> // ESP8266WiFi.h의 라이브러리를 포함하는 코드
#include <ESP8266WebServer.h> //ESP8266WebServer.h의 라이브러리를 포함하는 코드
#include <NewPing.h>//NewPing.h의 라이브러리를 포함하는 코드

const char *ssid = "KT_GiGA_Mesh_7C87"; //wifi의 이름을 지정한다.
const char *password = "czbagd3602";//wifi의 비밀 번호를 지정한다.

#define BOT_TOKEN "6936207643:AAFmNlFqu_xoG5g_dsH7e2xkJ-tOGiivWgw" //텔레그램의 Telegram Bot API에 대한 액세스 권한을 부여하는 고유 키인 BOT_TOKEN을 지정한다.
#define CHAT_ID "6925537602"  //텔레그램의 특정 채팅의 식별자를 나타내며 ESP8266 장치가 저에게 보내고 받을 수 있도록 해주는 'CHAT_ID'를 지정합니다.

#define TRIG_PIN D7 //TRIG_PIN을 D7로 지정한다.
#define ECHO_PIN D8//ECHO_PIN을 D8로 지정한다.
#define MAX_DISTANCE 200 
#define LED_PIN D6 //LED를 제어하는 핀의 부분을 D6으로 지정합니다.

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
ESP8266WebServer server(80);

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);

  // 컴퓨터 시간을 네트워크 시간과 동기화하는 설정
  configTime(0, 0, "pool.ntp.org");

  // 텔레그램 API (api.telegram.org)를 위한 루트 인증서 추가
  secured_client.setTrustAnchors(&cert);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  // Wi-Fi에 연결
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // 웹 서버 설정
  server.on("/", HTTP_GET, []() {
    unsigned int distance = sonar.ping_cm();

    // CORS 헤더 추가
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    // 거리에 따라 LED 업데이트
    if (distance > 20) { //거리가 20 이상이면
      digitalWrite(LED_PIN, LOW); // LED 끄기
      bot.sendMessage(CHAT_ID, "안전! 거리: " + String(distance) + "cm."); //안전 거리라는 문구와 측정된 거리를 같이 출력합니다.
    } else if (distance == 0) { // 거리가 0이면
      digitalWrite(LED_PIN, HIGH); // LED 켜기
      bot.sendMessage(CHAT_ID, "⚠️ 경고! 충돌 감지됨. 거리: " + String(distance) + "cm."); // ⚠️ 경고! 충돌 감지됨. 거리:와 거리를 같이 출력한다.
    }

    server.send(200, "text/plain", String(distance));
  });

  server.begin();
}

void loop() {
  server.handleClient();
  delay(1000); // 1초마다 웹 페이지 업데이트
  int numNewMsg = bot.getUpdates(bot.last_message_received + 1);

  for (int i = 0; i < numNewMsg; i++) {
    // 전송된 메시지에서 chat_id 찾기
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println(text);
    Serial.println(chat_id);

    if (text == "어떻게 급작스런 재난 상황에 대응하나요?") { //'어떻게 급작스런 재난 상황에 대응하나요?'를 입력하면
      bot.sendMessage(chat_id, "비상 시 음식 준비"); //"비상 시 음식 준비"라는 메시지를 보냅니다.
    }
    if (text == "재난 시 어떻게 해야 하나요") { //"재난 시 어떻게 해야 하나요"를 입력하면
      bot.sendMessage(chat_id, "빠르게 병원으로 이동"); //"빠르게 병원으로 이동"이라는 메시지를 보냅니다.
    }
  }
}
