#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Secret.h>

#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883            // MQTT Broker Port

// Define your MQTT topic constants
const char* MQTT_PUB_TEMP = "Backup_Heater_Control"; // Temperature MQTT Topic
const char* MQTT_PUB_HEAT = "Backup_Heater_Status";  // Heater Status MQTT Topic


const int oneWireBus = 4;
unsigned long previousMillis = 0;
unsigned long thenMillis = 0;
const long interval = 10000;
const long interval1 = 15000;
float temp = 0.0;
float minTempThreshold = 25.1;
float maxTempThreshold = 25.4;
const int heaterPin = 23;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  while (!mqttClient.connected()) {
    Serial.println("Attempting MQTT connection...");
    if (mqttClient.connect("ESP8266Client")) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe("backupMaxSetting");
      mqttClient.subscribe("backupMinSetting");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void WiFiEvent(WiFiEvent_t event) {
  switch (event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      connectToWifi();
      break;
  }
}

void setup() {
  Serial.begin(115200);
  sensors.begin();
  pinMode(2, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  digitalWrite(heaterPin, HIGH);

  WiFi.onEvent(WiFiEvent);
  connectToWifi();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);

  // Additional setup code here...
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sensors.requestTemperatures();
    temp = sensors.getTempCByIndex(0);
    mqttClient.publish(MQTT_PUB_TEMP, String(temp).c_str());
  }

  if (currentMillis - thenMillis >= interval1) {
    thenMillis = currentMillis;
    publishHeaterStatus();
  }

  controlHeater();

  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();
}

void controlHeater() {
  if (temp <= minTempThreshold) {
    digitalWrite(2, HIGH);
    digitalWrite(heaterPin, LOW);
  } else if (temp >= maxTempThreshold) {
    digitalWrite(2, LOW);
    digitalWrite(heaterPin, HIGH);
  }
}

void publishHeaterStatus() {
  if (digitalRead(heaterPin) == LOW) {
    mqttClient.publish(MQTT_PUB_HEAT, "Heater On");
  } else {
    mqttClient.publish(MQTT_PUB_HEAT, "Heater Off");
  }
}

void onMqttMessage(char* topic, byte* payload, unsigned int length) {
  if (strcmp(topic, "backupMaxSetting") == 0) {
    payload[length] = '\0'; // Null-terminate the payload
    float newMaxTemp = atof((const char*)payload);
    maxTempThreshold = newMaxTemp;
    Serial.print("Updated maxTempThreshold to: ");
    Serial.println(maxTempThreshold);
  }
  if (strcmp(topic, "backupMinSetting") == 0) {
    payload[length] = '\0'; // Null-terminate the payload
    float newMinTemp = atof((const char*)payload);
    minTempThreshold = newMinTemp;
    Serial.print("Updated minTempThreshold to: ");
    Serial.println(minTempThreshold);
  }
}
