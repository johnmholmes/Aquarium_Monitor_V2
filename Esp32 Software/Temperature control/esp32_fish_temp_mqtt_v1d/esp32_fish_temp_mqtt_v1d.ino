/* This is designed to be used with the DOIT ESP32 DEVKIT V1.
   This is my test version the original sketch I can not remember were I found it.
   I am only using 1 sensor at the momment but it can read more sensor.

   This sketch is still being developed and is used for demonstration purposes only for the YouTube
   vidoes. The most upto date version can be found on my github account
   https://github.com/johnmholmes/Aquarium_Monitor_V2

   
   */

//Include the following libraries required for this to workyou may need to add them to your libraries 

#include <WiFi.h>
extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Secret.h>
// End of libraries

//These are the variables that need to be setup by the user
#define MQTT_HOST IPAddress(192, 168, 0, 30)     // Raspberry Pi Mosquitto MQTT Broker
#define MQTT_PORT 1883                           // Pi port used
#define MQTT_PUB_TEMP "Backup_Heater_Control"    // Temperature MQTT Topic
#define MQTT_PUB_HEAT "Backup_Heater_Status"     // Temperature MQTT Topic
const int oneWireBus = 4;                        // GPIO where the DS18B20 is connected to
unsigned long previousMillis = 0;                // Stores last time temperature was published
unsigned long thenMillis = 0;
const long interval = 10000;                     // Interval at which to publish sensor readings ever 30 seconds
const long interval1 = 15000;                    // Interval to publish if heater is on or off
float temp = 0.0;                                // Temperature value
float minTempThreshold = 25.1;                   // Set the min temp value
float maxTempThreshold = 25.4;                   // Set the Max temp value
const int heaterPin = 23;                        // Pin for the heater control



//-------------------------------------------------------------------------------------------

//Setup the 1 wire to use the Ds18b20
OneWire oneWire(oneWireBus);                    // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);            // Pass our oneWire reference to Dallas Temperature sensor 
//-------------------------------------------------------------------------------------------

//Setup the WIFI and MQTT connections
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void connectToWifi() {
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

void connectToMqtt() {
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
      xTimerStart(wifiReconnectTimer, 0);
      break;
  }
}

void onMqttConnect(bool sessionPresent) {
}

void onMqttDisconnect(AsyncMqttClientDisconnectReason reason) {
  if (WiFi.isConnected()) {
    xTimerStart(mqttReconnectTimer, 0);
  }
}

void onMqttPublish(uint16_t packetId) {
}
//-------------------------------------------------------------------------------------------------


//Start of Board setup
void setup() {
  
  sensors.begin();              // Start the DS18B20 sensor
  pinMode(2, OUTPUT);           // setup an output on pin 2
  pinMode(heaterPin, OUTPUT);          // setup an output on pin 23
  Serial.begin(115200);

  digitalWrite(heaterPin, HIGH);

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWifi();
}

//---------------------------------------------------------------------------------------------

//Functions called from Main Loop

void publishTemperature() {
  sensors.requestTemperatures();
  temp = sensors.getTempCByIndex(0);
  uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str());
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
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_HEAT, 1, true, "Heater On");
  } else {
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_HEAT, 1, true, "Heater Off");
  }
}


// Start of Void Loop run forever
void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    publishTemperature();
  }

  if (currentMillis - thenMillis >= interval1) {
    thenMillis = currentMillis;
    publishHeaterStatus();
  }

  controlHeater();
}
