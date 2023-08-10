#  Aquarium Monitor V2: The Raspberry Pi 4 4GB - Next Generation of My Marine Tank Monitor

In this project, I will document my progress in designing and implementing the next generation of my marine tank monitor using the Raspberry Pi 4 with 4GB of RAM. This documentation will serve as a future reference, allowing me to easily revisit and make any necessary changes to the system. Moreover, I am sharing this journey with others, enabling them to embark on similar endeavors.

Please beaware I may well change my approach to this project but will leave the files here for future reference.

 -----

 # Latest News Will Appear Here

I have reached a decision to alter my approach for version 2 of the controller. Recent days of research have drawn my attention to the DS18B20 sensors and the potential issues they can introduce when connected to the Raspberry Pi. While I personally haven't encountered these issues, I believe it's prudent to consider them in my design and strategy.

Despite the change in direction, I intend to keep the files and videos accessible, as they could prove valuable for future reference by others.

My new focus will center on utilizing the Raspberry Pi as a standalone server. Individual modules will transmit data via MQTT to the Raspberry Pi, which will then distribute messages to other units subscribed to the pertinent topics.

This approach also opens up additional possibilities for IoT applications, such as home security, among others.

 ------

 # WhyI Am Making V2

The earlier version of the monitor had all components hardwired into the Raspberry Pi 4, posing challenges for maintenance and implementing design enhancements. Regrettably, during my vacation, a lightning strike occurred, rendering the Raspberry Pi inoperable. Simultaneously, the old PC power supply I was utilizing also malfunctioned, resulting in a loss of external connectivity. Consequently, I made the decision to develop a new and improved iteration (V2) that would possess enhanced durability and ease of maintenance, thereby ensuring the continuous functionality of the monitor in the future.

 ------

 # Controller Requirements

The primary objective of this project is to establish worldwide accessibility for monitoring selected items.

The system's design will encompass the monitoring of temperatures in the display tank, sump tank, and room. This data will be presented through a Node Red GUI.

To achieve heightened monitoring and safety protocols, an additional microcontroller (ESP32) will be employed. This microcontroller will oversee the display tank and activate an emergency heater set to a lower temperature (e.g., 23.5 degrees Celsius) than the standard tank temperature of 25.5 degrees Celsius.

Furthermore, the project will feature an auto top-off (ATO) system for the sump. This system will integrate optical infrared sensors and a Tunzi pump situated within a 25-liter barrel adjacent to the sump. An ESP32 will oversee both the sump and ATO barrel, relaying the collected data to the Raspberry Pi via MQTT, which will then be displayed on a Node Red Dashboard. The pump will be programmed to operate for a specific duration, necessitating only one sensor in the sump.

Additionally, the system will incorporate water level monitoring for the display tank to prevent overflow incidents. This functionality will empower the system to temporarily halt one of the return pumps, ensuring the water level remains within safe parameters. Control of this function will be assigned to an ESP32, equipped with a single optical infrared sensor.

Lastly, the project will involve the monitoring of the skimmer overflow container, automatically ceasing the supply to the skimmer when required.

Subsequent components will be incorporated as the project progresses.

-----

# Languages Used 

 The skills required to be able to program the Raspberry Pi will depend on what you are hoping to do, However in my case I will be using 3 different coding lamguages depending on the poticular requirements.

 Python

 This will be used in certain script to controll GPIO or even database insertion.

 JavaScritp

 This is used for Node Red it works great in fuction Nodes as well as altering the GUI appearance.

 C/C++

 This is used for controlling the external modules.

 I am not a proffesional programmer but self taught from various sorces Books, Internet tutorial sites and Youtube videos. I got back into coding only 5 years ago, before that it was basic programming for the old z81 
 computers and the Amiga's

 ------
 
# Work Completed

      1. Give acces to the Raspberry Pi via VNC so I can access and control the Pi from anywhere in the world.

----

# Temperature Monitoring

This ongoing aspect of the project is currently in progress due to various reasons, with the primary one being the evolving nature of the project's scope.

Temperature monitoring involves the Display tank, Sump tank, and Living Room, all facilitated through Node Red. This has been partially demonstrated in several videos. However, I am still in the process of determining the optimal approach for the final implementation. Presently, I'm grappling with the challenge of flow stalling caused by the available Nodes when working with the DS18B20 sensor. While flow stalling isn't my foremost concern at the moment, I am more troubled by the occurrence of false readings. I am engaged in a personal deliberation regarding whether I should transition all the DS18B20 sensors to a single ESP32, utilizing MQTT to transmit the data to the Raspberry Pi. Concurrently, I am dedicating myself to further learning in Java to investigate the possibility of filtering out these erroneous readings, thereby preventing their display on the graph.

Another facet of the project pertains to temperature monitoring solely in the Display tank, wherein an ESP32 is employed. This ESP32 also encompasses contingency control over a single heater within the tank, configured to activate if the Display temperature falls below 23.5 degrees Celsius. This secondary heater is equipped with a thermostat set at 25 degrees Celsius. This component is still a work in progress and under development.

----

# To Be Started

Auto top of via a ESP32 this will monitor the sump level and report to the Pi as well as running the Ato pump, this will have a fail safe built in via a float switch incase the ESP32 fails.

Display tank water level monitor to prevent over fill may osibly control on of the return pumps.

Leak detection.

Visual display via LEDs above the tank.

----



