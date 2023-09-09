# ATO Controller

This folder will hold various skecthes as I create them for the Esp32 Auto Top Off modules. 


The water proof optical infrared sensors are a cost effective way of getting the current water levels. However they can be damaged if not wired the correct way. The use a 5 volt supply to two of there wires IE white and blue. The red wire must be taken to ground or 0Volts through a 390 ohm resistor. If this is not done you will burnout the infrared led inside the unit. The yellow wire is the data output and this has to be wired with a 4.7k ohm resistor to ground and we take the reading between the resistor and the unit. When the sensor is in water it will have a state of about 0 volts and when out of water it will read about 5 volts.

These units will need to be cleaned every now and then to make sure they keep giving reliable results.

![Screenshot 2023-09-09 194709](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/ccd80f84-2e46-4943-81f1-084951e1fe7c)

![Screenshot 2023-09-09 194133](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/51f80b5c-c712-4b7b-85b5-c293fd5585b9)

There are also some 3 wire version available but finding a good example on using them seems very hard to find. The ones I got had different colours compared to some i seen on the ebay listing.

   1. Version 2 was the first pubished version which was working but I was not happy with it.
   2. Version 3 has been changed so that it only uses 2 topics to send out the current state of the Sump ad the ATO barrel. it still controls the ato pump on pin 5 for 10 secods. I will be creating the flow changes as well and add them to the relevent folder in the next few days. This will also need a change to be made for the display module to use the new mqtt payloads. The new module is being tested now to check for bugs.
































