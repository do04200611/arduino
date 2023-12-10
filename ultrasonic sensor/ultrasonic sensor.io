#include <UniversalTelegramBot.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NewPing.h>


const char *ssid = "KT_GiGA_Mesh_7C87";
const char *password = "czbagd3602";

#define BOT_TOKEN "6936207643:AAFmNlFqu_xoG5g_dsH7e2xkJ-tOGiivWgw"
#define CHAT_ID "6925537602"

#define TRIG_PIN D7
#define ECHO_PIN D8
#define MAX_DISTANCE 200
#define LED_PIN D6

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
ESP8266WebServer server(80);

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);

   //컴퓨터의 시간을 네트워크 시간과 맞추기 위한 설정
  configTime(0, 0, "pool.ntp.org");
  
  //텔레그램의 api에 대한 루트 인증서 추가(api.telegram.org)
  secured_client.setTrustAnchors(&cert);
  
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  // Set up the web server
  server.on("/", HTTP_GET, []() {
    unsigned int distance = sonar.ping_cm();

    // Add CORS headers
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Access-Control-Allow-Methods", "GET");
    server.sendHeader("Access-Control-Allow-Headers", "Content-Type");

    // Update LED based on distance
    if (distance > 20) {
      digitalWrite(LED_PIN, LOW);  // Turn off the LED
      bot.sendMessage(CHAT_ID, "Safe! Distance: " + String(distance) + "cm.");
    } else if (distance == 0) {
      digitalWrite(LED_PIN, HIGH);  // Turn on the LED
       bot.sendMessage(CHAT_ID, "⚠️ Warning! Collision detected. Object at " + String(distance) + "cm.");
    }

    server.send(200, "text/plain", String(distance));
  });

  server.begin();
}

void loop() {
  server.handleClient();
  delay(1000); // Update web page every 1 second
  int numNewMsg = bot.getUpdates(bot.last_message_received + 1);
  
  for(int i=0; i<numNewMsg; i++) {
    //전송된 메세지ㅇ서 chat_id 알아내기
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println(text);
    Serial.println(chat_id);

    if (text == " 재난상황에 어떻갑작스러운게 대처해?") {
      bot.sendMessage(chat_id, "비상시에 필요한 음식을 준비하세요");
    }
    if (text == "재난 발생 시 어떻게 해") {
      bot.sendMessage(chat_id, "병원에 빨리가세요");
    }
  }
}
