#include <OneWire.h>
#include <DallasTemperature.h>
#include <PubSubClient.h>
#include <Secret.h>  // Make sure you have Secret.h with WiFi credentials
#include <WiFi.h>

// Data wire is connected to GPIO4 (D4)
#define ONE_WIRE_BUS 4

const char* mqttServer = "192.168.0.30";
const int mqttPort = 1883;
const char* display = "displayTemp";
const char* sump = "sumpTemp";
const char* room = "roomTemp";

unsigned long restartTimer = 172800000;  // every 2 days


OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// Global variables to store sensor readings
float temperature1 = 0;
float temperature2 = 0;
float temperature3 = 0;

unsigned long previousMillis = 0;
unsigned long previousMillis1 = 0;

const long interval = 1800000;  // Interval in milliseconds (3 minutes which will make the graph more useful)

WiFiClient espClient;
PubSubClient client(espClient);

//---------------------------------------------------------------
void setup() {
  Serial.begin(115200);
  sensors.begin();

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");

  client.setServer(mqttServer, mqttPort);
  client.setKeepAlive(180);

  while (!client.connected()) {
    if (client.connect("ESP32Client")) {
      Serial.println("Connected to MQTT Broker");
    } else {
      Serial.print("MQTT Connection Failed, Retrying in 5 seconds...");
      delay(5000);
    }
  }
}


void espRebootTimer() {
  unsigned long currentMillis1 = millis(); 

  if (currentMillis1 - previousMillis1 >= restartTimer) {
    previousMillis1 = currentMillis1;
    ESP.restart();
  }
}


//-----------------------------------------------------------------------
void reconnect() {
  while (!client.connected()) {
    if (client.connect("billClient")) {
      Serial.println("Reconnected to MQTT Broker");
    } else {
      Serial.print("MQTT Reconnection Failed, Retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void updateTemperatures() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // Request temperature readings
    sensors.requestTemperatures();

    // Read temperatures from sensors
    temperature1 = sensors.getTempCByIndex(0);
    temperature2 = sensors.getTempCByIndex(1);
    temperature3 = sensors.getTempCByIndex(2);

    // Print temperatures to serial monitor
    Serial.print("Temperature 1: ");
    Serial.println(temperature1);
    Serial.print("Temperature 2: ");
    Serial.println(temperature2);
    Serial.print("Temperature 3: ");
    Serial.println(temperature3);

    // Publish temperatures to MQTT topics
    client.publish(display, String(temperature1).c_str());
    client.publish(sump, String(temperature2).c_str());
    client.publish(room, String(temperature3).c_str());
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  updateTemperatures();

  espRebootTimer();


}
