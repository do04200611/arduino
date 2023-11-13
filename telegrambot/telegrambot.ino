#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>

const char *ssid = "DIT_FREE_WiFi";
const char *password = "";

#define BOT_TOKEN "6936207643:AAFmNlFqu_xoG5g_dsH7e2xkJ-tOGiivWgw"
  #define CHAT_ID "6925537602"


//X509는 암호학에서 공개키 인증서와 인증알고리즘의 표준 가운데에서 공개 키 기반(PKI)의 ipu-t 표준
X509List cert(TELEGRAM_CERTIFICATE_ROOT);

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);
  
  //컴퓨터의 시간을 네트워크 시간과 맞추기 위한 설정
  configTime(0, 0, "pool.ntp.org");
  
  //텔레그램의 api에 대한 루트 인증서 추가(api.telegram.org)
  secured_client.setTrustAnchors(&cert);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP: "); Serial.println(WiFi.localIP());
}

void loop() {
  //chat id로 메시지 보내고 메시지 전송 여부를 변환
  bot.sendMessage(CHAT_ID, "Hello, World!", "");
  
  delay(10000); //10초
}
