#include <WiFi.h>
#include <PubSubClient.h>
#include "Secret.h"
#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883

const int skimmerSensor = 13;
const int pumpCutoff = 5;

const char* MQTT_PUB_CUTOFF = "SkimmerState";

String currentSkimmerState = "";  // To store skimmer state

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void connectToMqtt() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("skimmerCutOff")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void readAndDetermineStates() {
  // Read sensor values
  int skimmerHighValue = digitalRead(skimmerSensor);
  
  // Determine skimmerState based on sensor value
  if (skimmerHighValue == HIGH) {
    currentSkimmerState = "skimmer normal";
  } else {
    currentSkimmerState = "skimmer high";
  }
}

void controlPump() {
  if (currentSkimmerState == "skimmer high"){
    digitalWrite(pumpCutoff, LOW);
    
  }
  if (currentSkimmerState == "skimmer normal") {
    digitalWrite(pumpCutoff, HIGH);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pumpCutoff, OUTPUT);
  pinMode(skimmerSensor, INPUT_PULLUP);

  connectToWifi();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setKeepAlive(120);

  digitalWrite(pumpCutoff, HIGH);
  delay(5000);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  
  // Call the function to read sensor values and determine states
  readAndDetermineStates();

  // Call the function to control the pump based on conditions
  controlPump();

  // Publish displayState  to MQTT topics
  mqttClient.publish(MQTT_PUB_CUTOFF, currentSkimmerState.c_str());
  Serial.println(currentSkimmerState);
  delay(10000);

}