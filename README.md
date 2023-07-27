# Aquarium_Monitor_V2
 The Raspberry Pi 4 4 Gig next generation of monitor for my marine tank.

 This time around the Pi will be used in a different way than previously, this will be documented here to make it easier to build and diagnose issues if the arise.

 The Pi will only have 3 sensors attached to it directly and these will be DS18B20 water proof temperature sensors. This will require 1 wire commication to be setup using GPIO 4 Pin 7.
 This I feel is important as it allows me to take regular reading and store them on a database on the Pi. This will be done using CRON running seperate python files which will be added to this repository.

 This will make it easier to remove and maintaine the Raspberry Pi and allow better cooling compared to the previous controller system. Everything else will be done using either ESP32 or Raspberry Pi Pico W using MQTT,
  and the Pi $ will be the Broker using Node red to give a web brouser interface.

 ------

 # Software_the_Pi_4_will_use.

 The list of software that will be used is as follows:

 Built in software to the Raspberry Pi Operation System.

 1. VNC see the readme in the VNC folder.
 2. 1 wire  see the readme in the 1 Wire folder.

 We will need to install Node Red
