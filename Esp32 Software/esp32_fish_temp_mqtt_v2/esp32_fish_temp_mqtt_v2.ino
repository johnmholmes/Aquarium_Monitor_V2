/* This is designed to be used with the DOIT ESP32 DEVKIT V1.
   This is my test version the original sketch I can not remember were I found it.
   I am only using 1 sensor at the momment but it can read more sensor.

   This sketch is still being developed and is used for demonstration purposes only for the YouTube
   vidoes. The most upto date version can be found on my github account
   https://github.com/johnmholmes/Aquarium_Monitor_V2

   Since making the first video showing this I have changed my WIFI login and Password for security this
   requires a few extra lines of code to remove my details from the github pages. 
   
   #include <Secret.h>                              ADDED CODE

   const char ssid[] = WIFI_SSID;                   CHANGED CODE
   const char password[] = WIFI_PASSWORD;           CHANGED CODE

   WiFi.begin(ssid, password);                      CHANGED CODE

   See https://github.com/arduino-libraries/WiFi/tree/master for more details on wifi.h
   See https://github.com/maniacbug/FreeRTOS/blob/master/FreeRTOS.h see for more details on FreeRTOS.h
   See https://github.com/marvinroger/async-mqtt-client/tree/develop for more detail on async-mqtt-client.h 
   See https://github.com/PaulStoffregen/OneWire for more details on OneWire.h
   See https://github.com/milesburton/Arduino-Temperature-Control-Library for more details on the DallasTemp.h
   */

//Include the following libraries required for this to workyou may need to add them to your libraries 

#include <WiFi.h>
#include <Secret.h>
extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
// End of libraries

//These are the variables that need to be setup by the user
const char ssid[] = WIFI_SSID;
const char password[] = WIFI_PASSWORD;
#define MQTT_HOST IPAddress(192, 168, 0, 30)  // Change for your Pi Ip address
#define MQTT_PORT 1883                        // Pi port used
#define MQTT_PUB_TEMP "Esp_Heater_Control"    // Temperature  reading MQTT Topic
#define MQTT_PUB_HEAT "Esp_Heater_Status"     // Temperature heater status MQTT Topic
const int oneWireBus = 4;                     // GPIO pin that the DS18B20 is connected to
unsigned long previousMillis = 0;             // Stores last time temperature was published
unsigned long thenMillis = 0;                 // Used in the time to publish
const long interval = 60000;                  // Interval at which to publish sensor readings ever 60 seconds
const long interval1 = 65000;                 // Interval to publish if heater is on or off
float temp;                                   // Temperature value

//End of Variables

OneWire oneWire(oneWireBus);                  // Setup a oneWire instance to communicate with any OneWire devices

DallasTemperature sensors(&oneWire);          // Pass our oneWire reference to Dallas Temperature sensor 

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

void connectToWifi() {
  WiFi.begin(ssid, password);
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
      xTimerStop(mqttReconnectTimer, 0);   // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
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

void setup() {
  
  sensors.begin();              // Start the DS18B20 sensor
  pinMode(2, OUTPUT);           // setup Led on Pin 2 for heart beat
  pinMode(23, OUTPUT);          // setup a solid state realy to control 240 volts AC
  Serial.begin(115200);         // Used for debug only print statements removed already

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWifi();
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
    Serial.print(temp);
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str()); 
                           
  }
  if (temp <=25.1) {
    digitalWrite(2, HIGH);
    digitalWrite(23, LOW);
  }
  if (temp >= 25.5) {
    digitalWrite(2, LOW);
    digitalWrite(23, HIGH);
  }
  unsigned long nowMillis = millis();
  if (nowMillis - thenMillis >= interval1) {
        thenMillis = nowMillis;
        if(temp < 25.4){
        uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_HEAT, 1, true, "Esp Heater On");   
        }
        else{
        uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_HEAT, 1, true, "Esp Heater Off");  
        }
  }
  
}
