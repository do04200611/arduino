#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <NewPing.h>

const char *ssid = "KT_GiGA_Mesh_7C87";
const char *password = "czbagd3602";
#define TRIG_PIN D7
#define ECHO_PIN D8
#define MAX_DISTANCE 200
#define LED_PIN D6

NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);
ESP8266WebServer server(80);

void setup() {
  Serial.begin(115200);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

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
    } else if (distance == 0) {
      digitalWrite(LED_PIN, HIGH);  // Turn on the LED
    }

    server.send(200, "text/plain", String(distance));
  });

  server.begin();
}

void loop() {
  server.handleClient();
  delay(1000); // Update web page every 1 second
}
