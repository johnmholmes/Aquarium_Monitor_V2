# Aquarium_Monitor_V2
 The Raspberry Pi 4 4 Gig next generation of monitor for my marine tank.

 This time around the Pi 4 will be used in a different way than previously, this will be documented here to make it easier to build and diagnose issues if they arise.

 The Pi 4 will only have 3 sensors attached to it directly and these will be DS18B20 water proof temperature sensors. This will require 1 wire commication to be setup using GPIO 4 Pin 7.
 This I feel is important as it allows me to take regular reading and store them on a database on the Pi 4. This will be done using CRON running seperate python files which will be added to this repository.

 This will make it easier to remove and maintaine the Raspberry Pi and allow better cooling compared to the previous controller system. Everything else will be done using either ESP32 or Raspberry Pi Pico W using MQTT,
  and the Pi $ will be the Broker using Node red to give a web brouser interface.

 ------
# Why_the_old_system_failed

 By having the Raspberry Pi hard wired into everything I had used a PC power supply to give 3 different voltages 12 volt, 5 volts, and 3.3 volts. We had a lightning strike which has damaged the PC power Ssupply so the Pi 
  was overloaded trying to run everry thing for 5 volts and 3.3 volts. Lesson learnt from this is to ensure if using one of these ensure surge protection is used.

 ------

# Controller_Requirements

The first priority is to have access to the Pi from anywhere in the world.  Completed.

-----

# Work In Progress

Temperature monitoring of Display, Sump, and Room via the Raspberry Pi. Work in progress.

Temperature monitoring of the Display via a ESP32 with back up control of a single heater in the tank set to cut in if the Display temp drops below 23.5 Deg. This will use a heater with a thermerstat set at 25 Degs.

Auto top of via a ESP32 this will monitor the sump level and report to the Pi as well as running the Ato pump, this will have a fail safe built in via a float switch incase the ESP32 fails.

Display tank water level monitor to prevent over fill may osibly control on of the return pumps.

Leak detection.

Visual display via LEDs above the tank.

----



