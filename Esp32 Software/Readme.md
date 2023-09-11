# ESP32 Sketches

This folder will hold various skecthes as I create them for the Esp32 modules. 

The ESP32 is a 3.3V device, which means that all of its input and output pins are designed to operate with a maximum voltage of 3.3 volts. Connecting the ESP32 to a voltage source greater than 3.3 volts can damage the device, so it's important to use level shifters or voltage dividers when interfacing with higher voltage devices.

It's also important to note that the power supply for the ESP32 should be 3.3V DC. Some development boards or modules may have built-in voltage regulators that can accept a higher input voltage (such as 5V) and regulate it down to 3.3V for the ESP32, but it's always best to check the specifications of the specific device you are using to ensure proper voltage supply.

Pins 34 35 can only be used as input pins

GPIO pins with internal pullup resistors are 14, 16-23.

GPIO pins without internal pullup resistors are 13, 25-27. 32 and 33.

GPIO pib=ns HIGH during boot are pins 1,,3, 5, 6-11, 14 & 15.

See chat.openai.com/auth/login?next=%2Fchat

   1. The temperature modules I have two different units working on my system.
   2. The Ato unit is now added to the github following on from video 21.
   3. The simple display is also now uploaded but is going to be updated so this will see a number of versions.

As I add more devices the list will be added to.

































