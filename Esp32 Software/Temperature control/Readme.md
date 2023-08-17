# ESP32 Sketches

This folder will hold various skecthes I create for the Esp32 Temperature control modules. 

These nodes are going through an number of changes so be prepared for new features being added to them.

The esp32_fish_temp_mqtt_v1 is a single DS18B20 sensor on the 1 wire bus, and will drive a solid state relay to control a single back up heater in the main display tank.

--------------

# Version 1 BackUp controller The esp32_fish_temp_mqtt_v1

Version 1 did not get uploaded so we start with: 

1. Version 1a just had my wifi details added to a Secret.h file.
2. Version 1b This version I Added different functions to call from the main loop().
3. Version 1c This version seperated out items that had been done in the main loop().
4. Version 1d This version now uses the current state of pin 23 to send the heater status to MQTT.
5. Version 1e This was to allow the Node Red to send messages to change the upper and lower thresholds
6. Version 1f This version was to get the feedback to work with Asycnmqtt
7. Version 1g This version has done away with the AsycnMqtt library and just uses the #include <PubSubClient.h> libaray This now is almost the project completed
8. Version 1h This version is the final installment for this module, it now has if any changes to the 2 varibles these will be saved to the EEprom and a reboot timer to avoid the roll over of the millis() function timer. The changes to the variable will be applied on startup of the ESP32.
9. Version 1J Adressed an issues with the EEPROM routine which caused an issues this has been addressed and now the controller module is working, however it can control the tank temperature to within a few decimal places. Storing the changes in EEPROM and restoring them is more difficult. I have this code at the moment rebooting everyday to make sure it is stable I have also fixed the spikes in the grap on Node red. This is done by changing the names of the mqtt topics, as the previous Asyncmqtt library had left some previuos data on my Raspberry Pi.

--------------
# Version 1 Main Temperature monitor module The esp32_Main_Temp_v1

The first steps have been done and this moduel is reporting the 3 main temperature reading for the Display tank, sump tank, and the living room. I am not planning on doing anything fancy with this moduel.

1. Version 1 This version is to only monitor 3 DS18B20 sensors and report back to the Raspberry Pi. This is done and being tested at the moment it is reporting every 5 seconds for testing purposes this can be dropped down to every few minutes in the future. It will also reboot the esp32 to avoid millis roll over time issues this is happening every 2 days at the moment. It is a very basic sketch.

































