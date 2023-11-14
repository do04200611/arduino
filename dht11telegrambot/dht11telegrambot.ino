#include <DHT.h>
#include <UniversalTelegramBot.h>
#include <WiFiClientSecure.h>
#include <ESP8266WiFi.h>

const char *ssid = "DIT_CS_703";  
const char *password = "";

#define BOT_TOKEN "6936207643:AAFmNlFqu_xoG5g_dsH7e2xkJ-tOGiivWgw"
#define CHAT_ID "6925537602"

int dhtPin = D1;
X509List cert(TELEGRAM_CERTIFICATE_ROOT);
DHT dht(dhtPin, DHT11);

WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

void setup() {
  Serial.begin(115200);
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
  Serial.print("IP :");
  Serial.println(WiFi.localIP());
}

void loop() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  float DI = 0.81 * temp + 0.01 * humid * (0.99 * temp - 14.3) + 46.3;
  String a;

  Serial.print("temp:"); Serial.print(temp);
  Serial.print("humid: "); Serial.print("C\t");
  Serial.print("humid: "); Serial.print(humid);Serial.println("%");
  Serial.print("DI: "); Serial.print(DI);Serial.println("%");
   
  if(DI >= 68) {
    a = "온도는 " + String(temp) + "\xc2\xb0" + "C 그리고 습도는 " + String(humid) + "% " + String(DI) + "로 전원 쾌적";
  } else if(DI >= 70) {
    a = "온도는 " + String(temp) + "\xc2\xb0" + "C 그리고 습도는 " + String(humid) + "% " + String(DI) + "로 불쾌를 나타냄";
  } else if(DI >= 75) {
    a = "온도는 " + String(temp) + "\xc2\xb0" + "C 그리고 습도는 " + String(humid) + "% " + String(DI) + "로 10%정도 불쾌";
  } else if(DI >= 80) {
    a = "온도는 " + String(temp) + "\xc2\xb0" + "C 그리고 습도는 " + String(humid) + "% " + String(DI) + "로 50%정도 불쾌";
  } else if(DI >= 83) {
    a = "온도는 " + String(temp) + "\xc2\xb0" + "C 그리고 습도는 " + String(humid) + "% " + String(DI) + "로 전원 불쾌";
  } else {
    a = "온도는 " + String(temp) + "\xc2\xb0" + "C 그리고 습도는 " + String(humid) + "% "+"불쾌 지수는" + String(DI) + "로 매우 불쾌";
  }
  
  bot.sendMessage(CHAT_ID, a, "Markdown");

  delay(10000);
}
