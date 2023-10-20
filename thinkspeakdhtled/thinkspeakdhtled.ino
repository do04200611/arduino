#include <ESP8266WiFi.h>
#include <ThingSpeak.h>
#include <DHT.h>
#define Green_LED D1

DHT dht(D1, DHT11);
WiFiClient client;

//WiFi 연결 정보
const char *ssid     = "DIT_CS_703"; 
const char *password = "";

//ThingSpeak 연결 정보
unsigned long channel_id =  2305796;
const char *api_key = "D0EO8LVXH5SNLT45";

void setup() {
  Serial.begin(115200);
  pinMode(Green_LED, OUTPUT);
  WiFi.begin(ssid, password);
  dht.begin();

  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  //ThingSpeak와 연결 시작
  ThingSpeak.begin(client);
}

void loop() {
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();

  if(isnan(temp) || isnan(humid)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("  Humid: ");
  Serial.println(humid);

  //채널의 속성 데이터와 연결
  ThingSpeak.setField(1, temp);
  ThingSpeak.setField(2, humid);

  //ThingSpeak의 모든 필드에 데이터 기록
  int x = ThingSpeak.writeFields(2305796,"D0EO8LVXH5SNLT45");

  //기록이 잘 되었으면 응담이 200이고 ok(성공)
  if(x==200) {
    Serial.println("Channel update successful.");
    digitalWrite(Green_LED, HIGH);
    delay(1000);
    digitalWrite(Green_LED, LOW);
    delay(1000);
  } else {
    Serial.println("Problem updating channel. Http erro code " + String(x));
    digitalWrite(Green_LED, LOW);
  }

  //무료 계정은 15초 이상이어야 함
  delay(150);
}
