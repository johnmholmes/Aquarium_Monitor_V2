#include <WiFi.h>
#include <PubSubClient.h>
#include "Secret.h"
#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883

const int skimmerSensor = 13;
const int pumpCutoff = 14;
const int ledPin = 2;

const char* MQTT_PUB_CUTOFF = "SkimmerState";
const char* SKIMMER_HEART_TOPIC = "SkimmerHeartState";

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
      mqttClient.subscribe(SKIMMER_HEART_TOPIC);
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

void turnOnLedFor500ms() {
  digitalWrite(ledPin, HIGH);
  delay(500);
  digitalWrite(ledPin, LOW);
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Convert payload to a string
  String message = String((char*)payload, length);

  Serial.print("Received message on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(message);
  
  
  if (strcmp(topic, SKIMMER_HEART_TOPIC) == 0) {
    if (message == "on") {
      turnOnLedFor500ms();
    }
  }
}


void controlPump() {
  if (currentSkimmerState == "skimmer high"){
    digitalWrite(pumpCutoff, HIGH);
    //digitalWrite(ledPin, HIGH);
    
  }
  if (currentSkimmerState == "skimmer normal") {
    digitalWrite(pumpCutoff, LOW);
    //digitalWrite(ledPin, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pumpCutoff, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(skimmerSensor, INPUT_PULLUP);

  connectToWifi();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setKeepAlive(120);
  mqttClient.setCallback(callback);

  digitalWrite(pumpCutoff, LOW);
  digitalWrite(ledPin, LOW);
  delay(5000);
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();
  // Call the function to read sensor values and determine states
  readAndDetermineStates();

  // Call the function to control the pump based on conditions
  controlPump();

  // Publish displayState  to MQTT topics
  mqttClient.publish(MQTT_PUB_CUTOFF, currentSkimmerState.c_str());
  Serial.println(currentSkimmerState);
  delay(10000);

}