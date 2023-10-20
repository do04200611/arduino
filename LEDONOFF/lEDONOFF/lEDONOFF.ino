void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_BUILTIN, LOW);
    Serial.println("LEDOFF\n");

  delay(1000);
  digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("LEDON\n");
  delay(2000);
}
