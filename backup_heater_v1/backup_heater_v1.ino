//backup_heater_v1

#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Secret.h>

#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883            // MQTT Broker Port

const char* MQTT_PUB_TEMP = "backupHeaterControl"; // Temperature MQTT Topic
const char* MQTT_PUB_HEAT = "backupHeaterStatus";  // Heater Status MQTT Topic
const int oneWireBus = 4;
const int heaterPin = 23;
float temp = 0.0;

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
  Serial.print("backup_heater_v1 sketch");
  pinMode(2, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  digitalWrite(heaterPin, HIGH);

  Serial.println("Temperature Control System Initialized");

  WiFi.onEvent(WiFiEvent);
  connectToWifi();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);
}

void controlHeater() {
  if (temp <= 24.0) {  // Replace with your desired constant threshold
    digitalWrite(2, HIGH);
    digitalWrite(heaterPin, LOW);
  } else if (temp >= 25.0) {  // Replace with your desired constant threshold
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
  // Handle MQTT messages if needed
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }

  if (!mqttClient.connected()) {
    connectToMqtt();
  }

  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  mqttClient.publish(MQTT_PUB_TEMP, String(temp).c_str());

  delay(60000);  // Delay for 1 minute

  publishHeaterStatus();
  controlHeater();

  mqttClient.loop();
}
