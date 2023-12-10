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
#define MAX_DISTANCE 200 // 초음파 센서 측정의 최대 허용 거리를 정의합니다.
#define LED_PIN D6 //LED를 제어하는 핀의 부분을 D6으로 지정합니다.

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE); // 초음파 센서와의 상호 작용을 위한 NewPing 클래스의 인스턴스를 생성합니다.
ESP8266WebServer server(80); // HTTP 요청을 처리하기 위해 포트 80에서 동작하는 ESP8266WebServer 클래스의 인스턴스를 생성합니다.

X509List cert(TELEGRAM_CERTIFICATE_ROOT); // X509List 클래스의 인스턴스를 생성하고 Telegram API의 루트 인증서로 초기화합니다.
WiFiClientSecure secured_client; // 안전한 Wi-Fi 클라이언트를 생성합니다.
UniversalTelegramBot bot(BOT_TOKEN, secured_client); // Telegram 봇과 안전한 클라이언트를 사용하여 UniversalTelegramBot 클래스의 인스턴스를 생성합니다.

void setup() {
  Serial.begin(115200); //serial 모니터의 속도를 115200으로 지정한다.

  // 컴퓨터 시간을 네트워크 시간과 동기화하는 설정
  configTime(0, 0, "pool.ntp.org");

  // 텔레그램 API (api.telegram.org)를 위한 루트 인증서 추가
  secured_client.setTrustAnchors(&cert);

  pinMode(TRIG_PIN, OUTPUT); // 초음파 센서의 트리거 핀을 출력으로 설정합니다.
  pinMode(ECHO_PIN, INPUT); // 초음파 센서의 에코 핀을 입력으로 설정합니다.
  pinMode(LED_PIN, OUTPUT); // LED 제어 핀을 출력으로 설정합니다.

  WiFi.mode(WIFI_STA); // ESP8266을 Station 모드로 설정하여 Wi-Fi 연결을 수행합니다.
  WiFi.begin(ssid, password); // 지정된 SSID와 비밀번호를 사용하여 Wi-Fi에 연결합니다.

  // Wi-Fi에 지정된 SSID와 비밀번호를 사용하여 연결을 시도합니다.
  WiFi.begin(ssid, password);
  
  // Wi-Fi 연결 상태가 WL_CONNECTED가 아닌 동안 반복합니다.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // 0.5초의 지연을 추가합니다.
    Serial.print(".");  // 진행 상태를 표시하기 위해 점을 출력합니다.
  }
  Serial.println(""); // 새로운 줄을 출력하여 시리얼 모니터를 한 줄 아래로 이동시킵니다.
  Serial.println("WiFi connected");// WiFi connected를 serial모니터에 출력합니다.
  Serial.print("IP address: "); // 시리얼 모니터에 IP adress:를 출력합니다.
  Serial.println(WiFi.localIP()); // 연결된 Wi-Fi의 로컬 IP 주소를 시리얼 모니터에 출력합니다.

  server.on("/", HTTP_GET, []() { // 웹 서버가 "/" 경로로 HTTP GET 요청을 처리하는 핸들러를 등록합니다.
    unsigned int distance = sonar.ping_cm();  // 초음파 센서를 사용하여 거리를 센티미터로 측정합니다.

    // CORS 헤더 추가
    server.sendHeader("Access-Control-Allow-Origin", "*"); // 모든 도메인에서의 요청을 허용
    server.sendHeader("Access-Control-Allow-Methods", "GET"); // GET 메소드를 허용
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type"); // Content-Type 헤더를 허용

    // 거리에 따라 LED 업데이트
    if (distance > 20) { //거리가 20 이상이면
      digitalWrite(LED_PIN, LOW); // LED 끄기
      bot.sendMessage(CHAT_ID, "안전! 거리: " + String(distance) + "cm."); //안전 거리라는 문구와 측정된 거리를 같이 출력합니다.
    } else if (distance == 0) { // 거리가 0이면
      digitalWrite(LED_PIN, HIGH); // LED 켜기
      bot.sendMessage(CHAT_ID, "⚠️ 경고! 충돌 감지됨. 거리: " + String(distance) + "cm."); // ⚠️ 경고! 충돌 감지됨. 거리:와 거리를 같이 출력한다.
    }

    server.send(200, "text/plain", String(distance)); //HTTP 응답 코드 200(OK)를 반환하고, 텍스트 형식의 본문에 거리 정보를 포함하여 클라이언트에 응답을 보냅니다.
  });

  server.begin(); // 웹 서버를 시작합니다.
}

void loop() {
  server.handleClient(); // 웹 서버가 클라이언트 요청을 처리하고 응답하는 역할을 수행합니다.
  delay(1000); // 1초마다 웹 페이지 업데이트
  int numNewMsg = bot.getUpdates(bot.last_message_received + 1); // Telegram 봇에서 새로운 메시지 업데이트를 가져와서 처리합니다.

  // 새로운 메시지 업데이트 수만큼 반복하여 각 메시지를 처리합니다.
for (int i = 0; i < numNewMsg; i++) {
    String chat_id = bot.messages[i].chat_id; // 전송된 메시지에서 chat_id를 찾습니다.
    String text = bot.messages[i].text; // 전송된 메시지에서 텍스트를 가져와서 시리얼 모니터에 출력합니다.
    Serial.println(text);
    Serial.println(chat_id); // 전송된 메시지의 chat_id를 시리얼 모니터에 출력합니다.
   
  
    if (text == "어떻게 급작스런 재난 상황에 대응하나요?") { //'어떻게 급작스런 재난 상황에 대응하나요?'를 입력하면
      bot.sendMessage(chat_id, "비상 시 음식 준비"); //"비상 시 음식 준비"라는 메시지를 보냅니다.
    }
    if (text == "재난 시 어떻게 해야 하나요") { //"재난 시 어떻게 해야 하나요"를 입력하면
      bot.sendMessage(chat_id, "빠르게 병원으로 이동"); //"빠르게 병원으로 이동"이라는 메시지를 보냅니다.
    }
  }
}
