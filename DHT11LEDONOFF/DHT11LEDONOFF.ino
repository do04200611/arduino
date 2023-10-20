#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#define DHTPIN D4
#define DHTTYPE DHT11
#define LED D0

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  Serial.begin(115200);
  dht.begin();
  lcd.init();
  lcd.backlight();
  pinMode(LED, OUTPUT);
}

void loop() {
  float temp = dht.readTemperature();
  float humi = dht.readHumidity();
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print("    Humi: ");
  Serial.println(humi);

  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(temp);
  lcd.setCursor(0,1);
  lcd.print("Humi: ");
  lcd.print(humi);

  if (temp >= 29) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  if (temp <=28.0 ) {
    digitalWrite(LED, LOW);
  }
  
  
  delay(1000);
}
