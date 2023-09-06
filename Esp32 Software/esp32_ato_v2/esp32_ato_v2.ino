#include <WiFi.h>
#include <PubSubClient.h>
#include <Secret.h>

#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883            // MQTT Broker Port

const char* MQTT_PUB_SUMP_LOW = "SumpLowState";
const char* MQTT_PUB_SUMP_HIGH = "SumpHighState";
const char* MQTT_PUB_ATO_LOW = "AtoLowState";

const int sumpLowPin = 13;   
const int sumpHighPin = 12;  
const int atoLowPin = 14;    
const int ledPin = 2;  
const int pump = 5;

bool sumpLowState = false;
bool sumpHighState = false;
bool atoState = false;

const long interval = 10000;
const long topOff = 120000;
unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;

unsigned long RebootTimer = 43200000; //12 hour reboot

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
    if (mqttClient.connect("johnsClient")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(sumpLowPin, INPUT_PULLUP);
  pinMode(sumpHighPin, INPUT_PULLUP);
  pinMode(atoLowPin, INPUT_PULLUP);
  pinMode(ledPin, OUTPUT);
  pinMode(pump, OUTPUT);

  digitalWrite(pump, LOW);
  
  connectToWifi();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setKeepAlive(120);
}

void SumpRead(){
  int sumpLowState = digitalRead(sumpLowPin);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    if (sumpLowState) {
    mqttClient.publish(MQTT_PUB_SUMP_LOW, "Sump level low"); 
    Serial.println("Sump level low");
    } 
    else {
      mqttClient.publish(MQTT_PUB_SUMP_LOW, "Sump level Ok"); 
      Serial.println("Sump level Ok"); 
      }
  }
}

void SumpHighRead(){
  int sumpHighState = digitalRead(sumpHighPin);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis1 >= interval) {
    previousMillis1 = currentMillis;
    if (sumpHighState) {
    mqttClient.publish(MQTT_PUB_SUMP_HIGH, "Sump level OK");
    Serial.println("Sump level OK");  
    } 
    else {
      mqttClient.publish(MQTT_PUB_SUMP_HIGH, "Sump High level High"); 
      Serial.println("Sump high level High"); 
     }
  }
}

void AtoRead(){
  int atoState = digitalRead(atoLowPin);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= interval) {
    previousMillis2 = currentMillis;
    if (atoState) {
    mqttClient.publish(MQTT_PUB_ATO_LOW, "Ato level Low");  
    Serial.println("Ato level Low"); 
    } 
    else {
      mqttClient.publish(MQTT_PUB_ATO_LOW, "Ato level Ok");  
      Serial.println("Ato level Ok"); 
     }
  }
}

void SumpTopOff(){
  int atoState = digitalRead(atoLowPin);
  int sumpLowState = digitalRead(sumpLowPin);
  unsigned long currentMillis1 = millis();
  if (currentMillis1 - previousMillis3 >= topOff) {
    previousMillis3 = currentMillis1;
    if (sumpLowState == HIGH && atoState == LOW){
      digitalWrite(pump, HIGH);
      delay(10000);
      digitalWrite(pump, LOW);
    }
  }
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  if (millis() > RebootTimer){
    ESP.restart();
  }
  
  SumpRead();
  delay(100);
  SumpHighRead();
  delay(100);
  AtoRead();
  delay(200);
  SumpTopOff();
  delay(100);
  mqttClient.loop(); // Handle MQTT connection and communication
  delay(1000);       
}

