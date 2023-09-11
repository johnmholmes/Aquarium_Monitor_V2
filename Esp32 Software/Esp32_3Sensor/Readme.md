# ESP32 Sketches for 3 Ds18b20 sensors

The ESP32 is a 3.3V device, which means that all of its input and output pins are designed to operate with a maximum voltage of 3.3 volts. Connecting the ESP32 to a voltage source greater than 3.3 volts can damage the device, so it's important to use level shifters or voltage dividers when interfacing with higher voltage devices.

It's also important to note that the power supply for the ESP32 should be 3.3V DC. Some development boards or modules may have built-in voltage regulators that can accept a higher input voltage (such as 5V) and regulate it down to 3.3V for the ESP32, but it's always best to check the specifications of the specific device you are using to ensure proper voltage supply.

This sketch I used to get started with the ds18b20 sensors to just read the values in the serial monitor. This has now been added to the Temperature control folder

I am leaving it here as its a simple example.




























