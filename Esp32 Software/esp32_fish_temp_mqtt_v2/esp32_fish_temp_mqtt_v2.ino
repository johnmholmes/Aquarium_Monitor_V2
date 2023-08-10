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
#include <Secret.h> // See video how to setup this file.
extern "C" {
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
}
#include <AsyncMqttClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// End of libraries

// These are the variables that need to be setup by the user
const char ssid[] = WIFI_SSID;
const char password[] = WIFI_PASSWORD;
#define MQTT_HOST IPAddress(192, 168, 0, 30)  // Change for your Pi Ip address
#define MQTT_PORT 1883                        // Pi port used

/* The next two lines of code can be changed, but would also need to be changed further down
   in the sketch so make sure you understand where they are used before changing.
*/

#define MQTT_PUB_TEMP "Esp_Heater_Control"    // Temperature  reading MQTT Topic.
#define MQTT_PUB_HEAT "Esp_Heater_Status"    // Temperature heater status MQTT Topic

// This next line of code needs to stay the same so do not change pin allocation.

const int oneWireBus = 4;                     // GPIO pin that the DS18B20 is connected to

// The next 4 variables are used for the timing you can change the 3rd and 4th ones only
unsigned long previousMillis = 0;             // Stores last time temperature was published
unsigned long thenMillis = 0;                 // Used in the time to publish
const long interval = 30000;                  // Interval at which to publish sensor readings ever 60 seconds
const long interval1 = 65000;                 // Interval to publish if heater is on or off

// This one is important as it allows us to use floating point numbers
float temp;                                   // Temperature value

//End of Variables

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);                  

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);          

// Use AsycnMqttClient to hand the timers to make sure we do not try connecting to mqtt while connecting to wifi
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;

// Used to connect to the wifi when required
void connectToWifi() {
  WiFi.begin(ssid, password);
}

// Used to connect to mqtt when wehave a wifi connection
void connectToMqtt() {
  mqttClient.connect();
}

/* Case 1 is we have a wifi connection so connect to mqtt
   Case 2 is used when we do not have a wifi connection
*/ 
void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      xTimerStop(mqttReconnectTimer, 0);   
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


// start the void setup
void setup() {
  
  sensors.begin();              // Start the DS18B20 sensor
  pinMode(2, OUTPUT);           // setup Led on Pin 2 for heart beat
  pinMode(23, OUTPUT);          // setup a solid state realy to control 240 volts AC
  Serial.begin(115200);         // Used for debug only print statements removed already

  
  // pulls all the timer details into once item
  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  
  // Starts the wifi connection
  connectToWifi();
}

// End void setup

// Start the void loop

void loop() {

  // Setup the current time for millis
  unsigned long currentMillis = millis();

  // Check to see if its time to read the sensor & if so send a mqtt message to the Raspberry Pi 4
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sensors.requestTemperatures(); 
    temp = sensors.getTempCByIndex(0);
    Serial.print(temp);  // Used if the esp32 is connected to a usb lead the Arduino Ide serial monitor
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_TEMP, 1, true, String(temp).c_str()); 
                           
  }

  // This check the value of the temp and if equal to or below a set value turn on relay
  if (temp <=23.9) {
    digitalWrite(2, HIGH);     //Led on
    digitalWrite(23, HIGH);    //Heater relay on
    uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_HEAT, 1, true, "Esp Heater On"); 
  }

  // This check the value of the temp and if equal to or above a set value turn off relay
  if (temp >= 25.0) {
    digitalWrite(2, LOW);     //Led Off
    digitalWrite(23, LOW);    //Heater relay off
  }

  // used to send a status update to the raspberry pi if the heater is on or off
  unsigned long nowMillis = millis();
  if (nowMillis - thenMillis >= interval1) {
        thenMillis = nowMillis;
        if(temp <= 25.0){
        // do nothing  
        }
        else{
        uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_HEAT, 1, true, "Esp Heater Off");  
        }
  }
  
}
