#include <OneWire.h>
#include <DallasTemperature.h>

// Pin for the 1-Wire bus
const int oneWireBusPin = 4;  // You can choose any available GPIO pin

// Create a OneWire instance to communicate with the sensors
OneWire oneWire(oneWireBusPin);

// Pass the OneWire instance to the DallasTemperature library
DallasTemperature sensors(&oneWire);

void setup() {
  Serial.begin(115200);
  sensors.begin();  // Initialize the library
}

void loop() {
  sensors.requestTemperatures();  // Send command to all sensors to get temperatures
  
  // Read temperatures from each sensor
  float temp1 = sensors.getTempCByIndex(0);
  float temp2 = sensors.getTempCByIndex(1);
  float temp3 = sensors.getTempCByIndex(2);
  
  Serial.print("Sensor 1: ");
  Serial.print(temp1);
  Serial.println(" °C");
  
  Serial.print("Sensor 2: ");
  Serial.print(temp2);
  Serial.println(" °C");
  
  Serial.print("Sensor 3: ");
  Serial.print(temp3);
  Serial.println(" °C");
  
  delay(1000);  // Wait for a second before reading again
}

