
int pin = D1;
void setup() {
        pinMode(pin, OUTPUT); 
}
 
void loop()        {
        if (analogRead(A0) > 512) {
                digitalWrite(pin, HIGH);
        } else {
                digitalWrite(pin, LOW);
        }
}
