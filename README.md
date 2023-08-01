#  Aquarium Monitor V2: The Raspberry Pi 4 4GB - Next Generation of My Marine Tank Monitor

In this project, I will be documenting my progress in designing and implementing the next generation of my marine tank monitor using the Raspberry Pi 4 with 4GB RAM. This documentation will serve as a reference for myself in the future, making it easy to look back on and make any necessary changes to the system. Additionally, I am sharing this journey with others so they may attempt something similar for themselves.

 ------
# Why_I Am Making V2

The previous version of the monitor had everything hardwired into the Raspberry Pi 4, which made it challenging to maintain or implement any design improvements. Unfortunately, while I was on vacation, a lightning strike occurred, causing the Raspberry Pi to stop working. Additionally, the old PC power supply I had been using also failed, resulting in a loss of outside connectivity. Therefore, I decided to create a new version (V2) that would be more robust and easier to maintain, ensuring the monitor's uninterrupted functionality in the future.

 ------

# Controller_Requirements



-----

# Work Completed

Give acces to the Raspberry Pi via VNC so I can access and control the Pi from anywhere in the world.

Temperature monitoring of Display, Sump, and Room via the Raspberry Pi.

See the flow in Node red
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



