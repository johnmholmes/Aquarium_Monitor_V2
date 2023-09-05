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


const char* ATO_TOPIC = "AtoLowState";
const char* SUMP_HIGH_TOPIC = "SumpHighState";
const char* SUMP_LOW_TOPIC = "SumpLowState";
const char* DISP_HIGH_TOPIC = "DispHighState";

WiFiClient espClient;
PubSubClient client(espClient);

bool atoLevel = LOW;           // Store the state of ATO_TOPIC
bool sumpHighLevel = LOW;      // Store the state of SUMP_HIGH_TOPIC
bool sumpLowLevel = LOW;       // Store the state of SUMP_LOW_TOPIC
bool dispHighLevel = LOW;       // Store the state of DISP_HIGH_TOPIC

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

void callback(char* topic, byte* payload, unsigned int length) {
  String message = ""; 

  if (strcmp(topic, ATO_TOPIC) == 0) {
    message = String((char*)payload, length);
    atoLevel = (message == "Ato level Low") ? HIGH : LOW; // Ato level Low - Ato level OK
    Serial.println(atoLevel);
  }

  if (strcmp(topic, SUMP_HIGH_TOPIC) == 0) {
    message = String((char*)payload, length); 
    sumpHighLevel = (message == "Sump High level High") ? HIGH : LOW; // Sump High level High - Sump level OK 
    Serial.println(sumpHighLevel);
  }

  if (strcmp(topic, SUMP_LOW_TOPIC) == 0) {
    message = String((char*)payload, length); 
    sumpLowLevel = (message == "Sump level low") ? HIGH : LOW; //  Sump level low - Sump level OK 
    Serial.println(sumpLowLevel);
    }

  if (strcmp(topic, DISP_HIGH_TOPIC) == 0) {
    message = String((char*)payload, length); 
    dispHighLevel = (message == "Diplay level high") ? HIGH : LOW; //  Diplay level high - Display level OK 
    Serial.println(message);
    }

  updateLEDs();
}

void updateLEDs() {
  // Output 1: sump yellow led
  if (sumpLowLevel == HIGH && sumpHighLevel == HIGH) {
    digitalWrite(SUMP_LED_GREEN, LOW);
    digitalWrite(SUMP_LED_RED, LOW);
    digitalWrite(SUMP_LED_YELLOW, HIGH);
    }
  // Output 2: sump green led
  else if (sumpLowLevel == LOW && sumpHighLevel == HIGH) {
    digitalWrite(SUMP_LED_GREEN, HIGH);
    digitalWrite(SUMP_LED_YELLOW, LOW);
    digitalWrite(SUMP_LED_RED, LOW);
    }
  // Output 3: sump red led
  else if (sumpLowLevel == HIGH && sumpHighLevel == LOW) {
    digitalWrite(SUMP_LED_GREEN, LOW);
    digitalWrite(SUMP_LED_YELLOW, LOW);
    digitalWrite(SUMP_LED_RED, HIGH);
    }
  // output 4: Ato red led
  if (atoLevel == HIGH) {
    digitalWrite(ATO_LED_RED, HIGH);
  }
    else {
      digitalWrite(ATO_LED_RED, LOW);
    }

  // output 5: Display leds
  if (dispHighLevel == HIGH) {
    digitalWrite(DISP_LED_RED, HIGH);
    digitalWrite(DISP_LED_GREEN, LOW);
  }
    else {
      digitalWrite(DISP_LED_RED, LOW);
      digitalWrite(DISP_LED_GREEN, HIGH);

    }
  }


void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP32Client")) {
      Serial.println("connected");
      client.subscribe(ATO_TOPIC);
      client.subscribe(SUMP_HIGH_TOPIC);
      client.subscribe(SUMP_LOW_TOPIC);
      client.subscribe(DISP_HIGH_TOPIC);
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
