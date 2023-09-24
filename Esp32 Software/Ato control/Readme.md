# ATO Controller

This folder will hold various skecthes as I create them for the Esp32 Auto Top Off modules. 
The ESP32 is a 3.3V device, which means that all of its input and output pins are designed to operate with a maximum voltage of 3.3 volts. Connecting the ESP32 to a voltage source greater than 3.3 volts can damage the device, so it's important to use level shifters or voltage dividers when interfacing with higher voltage devices.

It's also important to note that the power supply for the ESP32 should be 3.3V DC. Some development boards or modules may have built-in voltage regulators that can accept a higher input voltage (such as 5V) and regulate it down to 3.3V for the ESP32, but it's always best to check the specifications of the specific device you are using to ensure proper voltage supply.


The water proof optical infrared sensors are a cost effective way of getting the current water levels. However they can be damaged if not wired the correct way. The use a 5 volt supply to two of there wires IE white and blue. The red wire must be taken to ground or 0Volts through a 390 ohm resistor. If this is not done you will burnout the infrared led inside the unit. The yellow wire is the data output and this has to be wired with a 4.7k ohm resistor to ground and we take the reading between the resistor and the unit. When the sensor is in water it will have a state of about 0 volts and when out of water it will read about 3.3 volts.

I have also used a 3.3 volt relay unit to operate the Tunzi water pump wia a 12 volt power supply this allows me to set the jumper on the Esp32 module board for 3.3volt rather than 5 volts.

These units will need to be cleaned every now and then to make sure they keep giving reliable results.

![Screenshot 2023-09-09 194709](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/ccd80f84-2e46-4943-81f1-084951e1fe7c)


![Screenshot 2023-09-23 085514](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/f563934d-137a-4146-9775-08bda36aef83)

There are also some 3 wire version available but finding a good example on using them seems very hard to find. The ones I got had different colours compared to some i seen on the ebay listing.

   1. Version 2 was the first pubished version which was working but I was not happy with it.
   2. Version 3 has been changed so that it only uses 2 topics to send out the current state of the Sump ad the ATO barrel. it still controls the ato pump on pin 5 for 10 secods. I will be creating the flow changes as well and add them to the relevent folder in the next few days. This will also need a change to be made for the display module to use the new mqtt payloads. The new module is being tested now to check for bugs. The two topics are "SumpState" which has 3 possible outputs "low, normal, and high" & "AtoState" which as 2 possible outputs "low water, high water".
   3. Version 4 is now the latest version and has had a heart beat led added and also added a over ride facility to the module in the event that the sensor failed to respond to the water level. this will only run the pump for 5 seconds, when triggered from Node Red.

----

# Node Red Flow

The latest version of the flow I am using is now added which is used for a simple dash board.


![Screenshot 2023-09-24 232544](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/4d9997c0-771f-43fb-87eb-4d3d216533e1)

























