#include <WiFi.h>
#include <PubSubClient.h>
#include "Secret.h"  // Include the Secret.h file for Wi-Fi credentials

#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883

const int SUMP_LED_YELLOW = 15;
const int SUMP_LED_GREEN = 2;
const int SUMP_LED_RED = 4;
const int ATO_LED_RED = 16;
const int DISP_LED_YELLOW = 17;
const int DISP_LED_GREEN = 5;
const int DISP_LED_RED = 18;

const char* ATO_TOPIC = "AtoState";
const char* SUMP_TOPIC = "SumpState";
const char* DISPLAY_TOPIC ="DisplayState";
const char* HEART_TOPIC = "DispHeartState";

String currentSumpState = ""; // To store sump state
String currentAtoState = "";  // To store ATO state
String currentDisplayState = "";  // To store ATO state
String currentHeartState ="";

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  pinMode(ATO_LED_RED, OUTPUT);
  pinMode(SUMP_LED_YELLOW, OUTPUT);
  pinMode(SUMP_LED_GREEN, OUTPUT);
  pinMode(SUMP_LED_RED, OUTPUT);
  pinMode(DISP_LED_YELLOW, OUTPUT);
  pinMode(DISP_LED_GREEN, OUTPUT);
  pinMode(DISP_LED_RED, OUTPUT);

  setup_wifi();
  client.setServer(MQTT_HOST, MQTT_PORT);
  client.setCallback(callback);
}

void setup_wifi() {
  delay(10);
  Serial.begin(115200);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_SSID); 

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
}

// waiting for meddages to arrive on the subscribed topic then updayes the states and calls the update led function
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";

  if (strcmp(topic, ATO_TOPIC) == 0) {
    message = String((char*)payload, length);
    currentAtoState = message;
  } else if (strcmp(topic, SUMP_TOPIC) == 0) {
    message = String((char*)payload, length);
    currentSumpState = message;
  }else if (strcmp(topic, DISPLAY_TOPIC) == 0) {
    message = String((char*)payload, length);
    currentDisplayState = message;
  }else if (strcmp(topic, HEART_TOPIC) == 0) {
    message = String((char*)payload, length);
    currentHeartState = message;
  }

  updateLEDs();
}

void updateLEDs() {
  // Control Display LED on pin 17
  if (currentHeartState == "on") {
    digitalWrite(DISP_LED_YELLOW, HIGH);
  } 
  if (currentHeartState == "off") {
    digitalWrite(DISP_LED_YELLOW, LOW);
  }

  // Control Ato LED on pin 16
  if (currentAtoState == "low water") {
    digitalWrite(ATO_LED_RED, HIGH);
  } 
  if (currentAtoState == "high water") {
    digitalWrite(ATO_LED_RED, LOW);
  }

  // Control SUMP LEDs on pins 15, 2, and 4
  if (currentSumpState == "low") {
    digitalWrite(SUMP_LED_YELLOW, HIGH);
    digitalWrite(SUMP_LED_GREEN, LOW);
    digitalWrite(SUMP_LED_RED, LOW);
  } else if (currentSumpState == "normal") {
    digitalWrite(SUMP_LED_YELLOW, LOW);
    digitalWrite(SUMP_LED_GREEN, HIGH);
    digitalWrite(SUMP_LED_RED, LOW);
  } else if (currentSumpState == "high") {
    digitalWrite(SUMP_LED_YELLOW, LOW);
    digitalWrite(SUMP_LED_GREEN, LOW);
    digitalWrite(SUMP_LED_RED, HIGH);
  }

  // control display LEDs on pins 5, and 18
  if (currentDisplayState == "normal") {
    digitalWrite(DISP_LED_GREEN, HIGH);
    digitalWrite(DISP_LED_RED, LOW);
  } else if (currentDisplayState == "high"){
    digitalWrite(DISP_LED_GREEN, LOW);
    digitalWrite(DISP_LED_RED, HIGH);
  }

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Display")) {
      Serial.println("connected");
      client.subscribe(ATO_TOPIC);
      client.subscribe(SUMP_TOPIC);
      client.subscribe(DISPLAY_TOPIC);
      client.subscribe(HEART_TOPIC);


    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

