# ESP32 Sketches

This folder will hold various skecthes I create for the Esp32 modules for temperature monitoring. 

These nodes are going through an number of changes so be prepared for new features being added to them.

The esp32_fish_temp_mqtt_v2 is a single DS18B20 sensor on the 1 wire bus, and will drive a solid state relay to control a single back up heater in the main display tank.

The sketch is at the moment going through testing with some items of the code being improved during these test. The file is not here at the moment and will be added tomorrow once these changes have been made.

Work required is to remove the code from the void loop and add them to seperate functions. also iI want to narrow down the lower and uper limits to 0.3 of a DEG C. At the present its being tested at 0.5 of a DEG C. This is taking about 1 hour for the single heater to get the tank back up to temperature and 1 hour 30 minutes to drop down to the lower limit.

Also I want to extend the times between the sensor reading to 5 minutes.






























