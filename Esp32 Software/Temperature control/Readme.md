# ESP32 Sketches

This folder will hold various skecthes I create for the Esp32 Temperature control modules. 

These nodes are going through an number of changes so be prepared for new features being added to them.

The esp32_fish_temp_mqtt_v1 is a single DS18B20 sensor on the 1 wire bus, and will drive a solid state relay to control a single back up heater in the main display tank.

--------------

# Version 1 BackUp controller

Version 1 did not get uploaded so we start with: 

1. Version 1a just had my wifi details added to a Secret.h file.
2. Version 1b This version I Added different functions to call from the main loop().
3. Version 1c This version seperated out items that had been done in the main loop().
4. Version 1d This version now uses the current state of pin 23 to send the heater status to MQTT.
5. Version 1e This was to allow the Node Red to send messages to change the upper and lower thresholds
6. Version 1f This version was to get the feedback to work with Asycnmqtt
7. Version 1g This version has done away with the AsycnMqtt library and just uses the #include <PubSubClient.h> libaray This now is almost the project completed

--------------
# Version 2 Main control and monitor module



































