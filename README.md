#  Aquarium Monitor V2: The Raspberry Pi 4 4GB - Next Generation of My Marine Tank Monitor

In this project, I will be documenting my progress in designing and implementing the next generation of my marine tank monitor using the Raspberry Pi 4 with 4GB RAM. This documentation will serve as a reference for myself in the future, making it easy to look back on and make any necessary changes to the system. Additionally, I am sharing this journey with others so they may attempt something similar for themselves.

 ------
# WhyI Am Making V2

The previous version of the monitor had everything hardwired into the Raspberry Pi 4, which made it challenging to maintain or implement any design improvements. Unfortunately, while I was on vacation, a lightning strike occurred, causing the Raspberry Pi to stop working. Additionally, the old PC power supply I had been using also failed, resulting in a loss of outside connectivity. Therefore, I decided to create a new version (V2) that would be more robust and easier to maintain, ensuring the monitor's uninterrupted functionality in the future.

The new controller will only have a small number of items hardwired to it. Specifically, there will be 3 DS18B20 waterproof temperature sensors utilizing the 1-wire protocol on the Raspberry Pi 4. The breakout board will connect the bus wire to Pin 4 on the Pi, and I will draw 3.3 volts from Pin 1 while grounding will be supplied from Pin 6.

Additionally, I plan to incorporate an LED on Pin 40 with a ground connection on Pin 34. To protect the LED from the 3.3-volt supply from the Pi, I will use a 220-ohm resistor to proctect it.

 ------

# Controller Requirements

Access from anywhere in the world to monitor the items I choose is the primary goal of this project.

The system will be designed to monitor the display tank, sump tank, and room temperatures, presenting this data in a Node Red GUI.

To achieve enhanced monitoring and safety measures, a second microcontroller (ESP32) will be utilized. This microcontroller will monitor the display tank and activate an emergency heater, set at a lower temperature (e.g., 23.5 degrees Celsius) than the standard tank temperature of 25.5 degrees Celsius.

The project will also incorporate an auto top-off (ATO) system for the sump, which will utilize optical infrared sensors and a Tunzi pump in a 25-liter barrel located next to the sump. An ESP32 will monitor both the sump and the ATO barrel, communicating the data back to the Raspberry Pi via MQTT and displaying it on a Node Red Dashboard. The pump will be set to run for a designated period, requiring only one sensor in the sump.

Furthermore, the system will include water level monitoring for the display tank to prevent overflows. This functionality will enable the system to turn off one of the return pumps for a specified duration, ensuring the water level remains within safe limits. An ESP32 with one optical infrared sensor will be responsible for this control.

Lastly, the project will incorporate skimmer overflow container monitoring, automatically cutting off the supply to the skimmer when necessary.

More items will be added later on.

-----

# Work Completed

Give acces to the Raspberry Pi via VNC so I can access and control the Pi from anywhere in the world.

Temperature monitoring of Display, Sump, and Room via the Raspberry Pi.


----

# Work In Progress



Temperature monitoring of the Display via a ESP32 with back up control of a single heater in the tank set to cut in if the Display temp drops below 23.5 Deg. This will use a heater with a thermerstat set at 25 Degs. Work in Progress

----

# To Be Started

Auto top of via a ESP32 this will monitor the sump level and report to the Pi as well as running the Ato pump, this will have a fail safe built in via a float switch incase the ESP32 fails.

Display tank water level monitor to prevent over fill may osibly control on of the return pumps.

Leak detection.

Visual display via LEDs above the tank.

----



