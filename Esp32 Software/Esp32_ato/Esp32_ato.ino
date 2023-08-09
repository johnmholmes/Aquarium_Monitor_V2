/*The sketch is being developed to replace some of the sensors being controlled by the Raspbeery Pi 4 and will use MQTT to transfer
 *the sensor status to the Pi 4 
 *
 *MQTT and WiFi work is complete
 *Heart beat notification complete
 *
 *The EN, VN, VP pins must not be used in this configuation
 *
 *Work in progress with the first and seconed sensor check code being written.
 *
 */

/*
Libraries used to make this sketch work
*/

#include <WiFi.h>                                   //Wifi libarary used
#include <AsyncMqttClient.h>                        /*MQTT publish library can be used to subscribe as well. This file needs to be 
                                                      downloaed it can be found in my github ref 
                                                      https://github.com/johnmholmes/Arduinio-libraries-used
                                                    */
#include "Secret.h" 

/*
Variables used for WiFi and MQTT used by the libraries
*/

const char ssid[] = WIFI_SSID;
const char password[] = WIFI_PASSWORD;
#define MQTT_HOST IPAddress(192, 168, 0, 30)        // Raspberry Pi Mosquitto MQTT Broker address
#define MQTT_PORT 1883                              // Raspery Pi Port number do not alter
#define MQTT_PUB_LEVEL_CHECK "ATO_Sump_Check"       // This gives the topic name used to transmit the message
#define MQTT_PUB_HEART_BEAT "ATO_Heart_Beat"        // The payload is decided in the void loop depending on what you wish to send
#define MQTT_PUB_Container "ATO_Container_Check"    // The payload is decided in the void loop depending on what you wish to send

/*
Variables for the GPIO pins used for sensors and LED
*/

const int HeartBeatLed = 2;                         //Built in LED or add a seperate led for box used for heart beat
const int SumpLow  = 4;                             //Optiical infra red sensor 
const int AtoContainerLow  = 5;                     //Optiical infra red sensor 
const int AtoPump = 23;                             //Now added to the sketch and tested positioned away from the sensors

/* 
Sensor state variables
*/

int SumplowState = 0;
int AtoContainerLowState = 0;

/*
These variables are used for the various timers
*/

const long pumpRun = 10000;                         //10 Second ato pump run this will need adjusting once final testing done
const long interval = 60000;                        //1 minute timer for the hear beat led message and flash
const long longinterval = 3600000;                  //1 hour timer for auto top off check 3600000
unsigned long RebootTimer = 86400000;               //24 hours between reboots
unsigned long hbcurrentMillis = 0;                  //Heart beat timer
unsigned long hbpreviousMillis = 0;                 //Heart beat timer 
unsigned long SumpcurrentMillis = 0;                //Sump low level check timer 
unsigned long SumppreviousMillis = 0;               //Sump low level timer
unsigned long atocurrentMillis = 0;                 //Ato container level check timer 
unsigned long atopreviousMillis = 0;                //Ato container level timer

/*
Setup for WiFi and Mqtt so do not alter any of these setting
*/

AsyncMqttClient mqttClient;                         // Used to control the MQTT message transmition for the library to work
TimerHandle_t mqttReconnectTimer;                   // Do not mess with these
TimerHandle_t wifiReconnectTimer;

void connectToWifi() {
  WiFi.begin(ssid, password);
}                                                   // Starts the Wifi connection and supplys the login and password
 
void connectToMqtt() {
  mqttClient.connect();
}

void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
      connectToMqtt();
      break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
      xTimerStop(mqttReconnectTimer, 0);            // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
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

/*
Void Setup
*/

void setup() {


  pinMode(HeartBeatLed, OUTPUT);
  pinMode(AtoPump, OUTPUT);
  pinMode(SumpLow, INPUT_PULLUP);                      
  pinMode(AtoContainerLow, INPUT_PULLUP);

  Serial.begin(115200);

  mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
  wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

  WiFi.onEvent(WiFiEvent);

  mqttClient.onConnect(onMqttConnect);
  mqttClient.onDisconnect(onMqttDisconnect);
  mqttClient.onPublish(onMqttPublish);
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  connectToWifi();
}

/*
Functions to be called from the void loop()
*/

/* Heart beat timer should send a message every 60 seconds to the raspberry Pi via MQTT and blink the onboard LED on and off
 for 1/2 second before resetting the next time it will run. 
 */

void HeartBeat(){
  unsigned long hbcurrentMillis = millis();

  if (hbcurrentMillis - hbpreviousMillis >= interval) {

      uint16_t packetIdPub0 = mqttClient.publish(MQTT_PUB_HEART_BEAT, 1, true, "ATO Esp32 Working"); 
      digitalWrite(HeartBeatLed, HIGH);
      delay(500);
      digitalWrite(HeartBeatLed, LOW);
      hbpreviousMillis = hbcurrentMillis;
  }
}

/*
Container 
*/

void ContainerCheck(){
  unsigned long atocurrentMillis = millis();
  AtoContainerLowState = digitalRead(AtoContainerLow);
  
  if (atocurrentMillis - atopreviousMillis >= longinterval) {
    uint16_t packetIdPub0 = mqttClient.publish(MQTT_PUB_Container, 1, true, "Container checked"); 
    delay(500);
    atopreviousMillis = atocurrentMillis;

    if (AtoContainerLowState == LOW){
        uint16_t packetIdPub0 = mqttClient.publish(MQTT_PUB_Container, 1, true, "Container sensor out of water");
        }
      else{
        uint16_t packetIdPub0 = mqttClient.publish(MQTT_PUB_Container, 1, true, "Container sensor in water");
      }
  }
}

/* 
Refill function
*/

void Refill(){

  unsigned long SumpcurrentMillis = millis();
  SumplowState = digitalRead(SumpLow);
  AtoContainerLowState = digitalRead(AtoContainerLow);

  if (SumpcurrentMillis - SumppreviousMillis >= longinterval) {
      uint16_t packetIdPub0 = mqttClient.publish(MQTT_PUB_LEVEL_CHECK, 1, true, "Sump Water level Check");
      digitalWrite(HeartBeatLed, HIGH);
      delay(500);
      digitalWrite(HeartBeatLed, LOW);
      delay(500);
      SumppreviousMillis = SumpcurrentMillis;

      if ((SumplowState == LOW) && (AtoContainerLowState == HIGH)){
        uint16_t packetIdPub1 = mqttClient.publish(MQTT_PUB_LEVEL_CHECK, 1, true, "Sump sensor out of water");
        digitalWrite(AtoPump, HIGH);
        delay(200);
        uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_LEVEL_CHECK, 1, true, "Pump running");
        delay(pumpRun);
        digitalWrite(AtoPump, LOW);
        uint16_t packetIdPub3 = mqttClient.publish(MQTT_PUB_LEVEL_CHECK, 1, true, "Pump stopped");
      }
      
  }
}  

/*
Reboot ESP32 every 24 hours
*/

void RebootEsp(){
  if (millis() > RebootTimer){
    uint16_t packetIdPub2 = mqttClient.publish(MQTT_PUB_LEVEL_CHECK, 1, true, "Rebooting");
    delay(10000);   //delay 10 seconds to allow messsage to be sent before rebbot.
    ESP.restart();
   }
  
}

/*
Void Loop Start
*/

void loop(){

   HeartBeat();
   ContainerCheck();
   Refill();
   RebootEsp();
     
}
