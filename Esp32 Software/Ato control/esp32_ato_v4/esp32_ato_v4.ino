#include <WiFi.h>
#include <PubSubClient.h>
#include <Secret.h>

#define MQTT_HOST "192.168.0.30"  // MQTT Broker IP Address
#define MQTT_PORT 1883            // MQTT Broker Port

const char* MQTT_PUB_SUMP = "SumpState";
const char* MQTT_PUB_ATO = "AtoState";
const char* MQTT_SUB_ATO_OVERRIDE = "atoOverride"; 

const int sumpLowPin = 13;
const int sumpHighPin = 12;
const int atoLowPin = 27;
const int pump = 5;

const char* currentSumpState = "";
const char* currentAtoState = "";

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
    // the mqttClient.connect (needs to be a unique name for each device on the network)
    if (mqttClient.connect("johnsClient2")) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe(MQTT_SUB_ATO_OVERRIDE);
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" retrying in 5 seconds");
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  // Handle the MQTT message received on the subscribed topic
  String message = String((char*)payload, length);
    
  Serial.print("Received message on topic: ");
  Serial.print(topic);    Serial.print(", Message: ");
  Serial.println(message);
    
    if (message == "on") {
      // Turn on the pump for 5 seconds
      digitalWrite(pump, HIGH);
      delay(5000);
      digitalWrite(pump, LOW);
    
  }
}

void readAndDetermineStates() {
  // Read sensor values
  int sumpLowValue = digitalRead(sumpLowPin);
  int sumpHighValue = digitalRead(sumpHighPin);
  int atoLowValue = digitalRead(atoLowPin);

  // Determine sumpState based on sensor values
  if (sumpLowValue == HIGH) {
    currentSumpState = "low";
  } else if (sumpLowValue == LOW && sumpHighValue == HIGH) {
    currentSumpState = "normal";
  } else if (sumpHighValue == LOW) {
    currentSumpState = "high";
  }

  // Determine atoState based on sensor value
  if (atoLowValue == HIGH) {
    currentAtoState = "low water";
  } else {
    currentAtoState = "high water";
  }
}

void controlPump() {
  if (strcmp(currentSumpState, "low") == 0 && strcmp(currentAtoState, "high water") == 0) {
    // Turn on pin 5 for 10 seconds
    digitalWrite(pump, HIGH);
    delay(10000);  // 10 seconds
    digitalWrite(pump, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(sumpLowPin, INPUT_PULLUP);
  pinMode(sumpHighPin, INPUT_PULLUP);
  pinMode(atoLowPin, INPUT_PULLUP);
  pinMode(pump, OUTPUT);

  digitalWrite(pump, LOW);
  connectToWifi();
  mqttClient.setServer(MQTT_HOST, MQTT_PORT);
  mqttClient.setCallback(callback);
  mqttClient.setKeepAlive(60);  // Adjust as needed (e.g., 60 seconds)

  
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWifi();
  }
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  if (mqttClient.connected()) {
    mqttClient.loop();
  }

  readAndDetermineStates();
  controlPump();
  mqttClient.publish(MQTT_PUB_SUMP, currentSumpState);
  mqttClient.publish(MQTT_PUB_ATO, currentAtoState);

  

  delay(10000);
}
