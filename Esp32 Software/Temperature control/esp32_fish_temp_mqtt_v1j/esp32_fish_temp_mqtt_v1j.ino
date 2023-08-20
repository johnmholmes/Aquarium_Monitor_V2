#include <WiFi.h>
#include <PubSubClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Secret.h>
#include <EEPROM.h>


#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883            // MQTT Broker Port


const char* MQTT_PUB_TEMP = "backupHeaterControl"; // Temperature MQTT Topic
const char* MQTT_PUB_HEAT = "backupHeaterStatus";  // Heater Status MQTT Topic
const int oneWireBus = 4;
unsigned long previousMillis = 0;
unsigned long thenMillis = 0;
const long interval = 10000;
const long interval1 = 15000;
float temp = 0.0;
float minTempThreshold = 24.6;
float maxTempThreshold = 25.0;

const int heaterPin = 23;

// These are used for the storing of the EEPROM only when the variables have been changed.
unsigned long restartTimer = 86400000; // 28 day reset timer 2419200000
float lastMinTempThreshold = minTempThreshold;  // Track last-written values
float lastMaxTempThreshold = maxTempThreshold;

OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

WiFiClient espClient;
PubSubClient mqttClient(espClient);

void espRebootTimer() {
  if ((millis() % restartTimer) < 2000) {
    if (minTempThreshold != lastMinTempThreshold || maxTempThreshold != lastMaxTempThreshold) {
      EEPROM.write(0, int(minTempThreshold ));
      EEPROM.write(1, int(maxTempThreshold ));
      EEPROM.commit();
      EEPROM.begin(512);
      lastMinTempThreshold = minTempThreshold;
      lastMaxTempThreshold = maxTempThreshold;
    }
    ESP.restart();
  }
}

void restoreFromEEPROM() {
  minTempThreshold = float(EEPROM.read(0));
  maxTempThreshold = float(EEPROM.read(1));
  lastMinTempThreshold = minTempThreshold;
  lastMaxTempThreshold = maxTempThreshold;
}

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
  EEPROM.begin(512);
  float storedMinTemp = EEPROM.read(0);
  float storedMaxTemp = EEPROM.read(1);

  if (storedMinTemp == 0xFF && storedMaxTemp == 0xFF) {

    EEPROM.write(0, minTempThreshold);
    EEPROM.write(1, maxTempThreshold);
    EEPROM.commit();
}   else {
      minTempThreshold = storedMinTemp;
      maxTempThreshold = storedMaxTemp;
}
 
  restoreFromEEPROM();
  pinMode(2, OUTPUT);
  pinMode(heaterPin, OUTPUT);
  digitalWrite(heaterPin, HIGH);

  Serial.println(minTempThreshold);
  Serial.println(maxTempThreshold);

  WiFi.onEvent(WiFiEvent);
  connectToWifi();

  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(onMqttMessage);

  
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

    // Send a return message with the updated setting
    char returnMessage[50]; // Adjust the size as needed
    snprintf(returnMessage, sizeof(returnMessage), "New maxTempThreshold: %.2f", maxTempThreshold);
    mqttClient.publish("feedbackTopic", returnMessage);
  }
  if (strcmp(topic, "backupMinSetting") == 0) {
    payload[length] = '\0'; // Null-terminate the payload
    float newMinTemp = atof((const char*)payload);
    minTempThreshold = newMinTemp;
    Serial.print("Updated minTempThreshold to: ");
    Serial.println(minTempThreshold);

    // Send a return message with the updated setting
    char returnMessage[50]; // Adjust the size as needed
    snprintf(returnMessage, sizeof(returnMessage), "New minTempThreshold: %.2f", minTempThreshold);
    mqttClient.publish("feedbackTopic", returnMessage);
  }
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
  espRebootTimer();
}
