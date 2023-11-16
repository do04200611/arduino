#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>

const char* ssid = "KT_GiGA_Mesh_7C87";// 무선 네트워크 SSID
const char* password = "czbagd3602"; 

#define BOT_TOKEN "6936207643:AAFmNlFqu_xoG5g_dsH7e2xkJ-tOGiivWgw"
#define CHAT_ID "6925537602"


int green = D5;
int red = D3;
int yellow = D2;
int buzzer = D1; // Piezo 부저에 연결된 핀


X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);
  pinMode(green, OUTPUT);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(buzzer, OUTPUT);

  configTime(0, 0, "pool.ntp.org");

  secured_client.setTrustAnchors(&cert);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP :"); Serial.println(WiFi.localIP());
  
}

void Song() {
   tone(buzzer, 262, 500);
    delay(500);
     tone(buzzer, 294, 500);
    delay(500);
     tone(buzzer, 330, 500);
    delay(500);
     tone(buzzer, 349, 500);
    delay(500);
     tone(buzzer, 392, 500);
    delay(500);
     tone(buzzer, 440, 500);
    delay(500);
     tone(buzzer, 494, 500);
    delay(500);
     tone(buzzer, 523, 500);
    delay(500);
}



void loop() {
  int numNewMsg = bot.getUpdates(bot.last_message_received + 1);

  for(int i=0; i<numNewMsg; i++) {
    //전송된 메세지ㅇ서 chat_id 알아내기
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;
    Serial.println(text);
    Serial.println(chat_id);

    if (text == "green") {
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
      digitalWrite(yellow, LOW);
      bot.sendMessage(chat_id, "green LED turned Off");
    }
    if (text == "red") {
      digitalWrite(red, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(yellow, LOW);
      bot.sendMessage(chat_id, "red LED turned Off");
    }
    if (text == "yellow") {
      digitalWrite(yellow, HIGH);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      bot.sendMessage(chat_id, "yellow LED turned Off");
    }
    if (text == "off") {
      digitalWrite(yellow, HIGH);
      digitalWrite(green, HIGH);
      digitalWrite(red, HIGH);
      bot.sendMessage(chat_id, "all LED turned off");
    }
    if (text == "christmas") {
      digitalWrite(yellow, LOW);
      digitalWrite(green, LOW);
      digitalWrite(red, LOW);
      bot.sendMessage(chat_id, "all LED turned on");
    }
    // "Pirates" 명령어가 수신되면 캐리비안 해적 멜로디 연주
    if (text.equalsIgnoreCase("Songs")) {
      Song();
       digitalWrite(buzzer, LOW);
      bot.sendMessage(chat_id, "mellody");
    }
  }

}
