# ESP32 Auto Top Off Module

The sketch is starting to be developed in a series of small steps. The plan is, it will incorporate one sensor to monitor the sump return chamber, along with one sensor to monitor the ATO container. The pump I use is a Tunzi ATO pump as this fits through  the container opening. This is powered by 12 volts power supply, which will also power the Esp32, 

Adjusting the pump's runtime will involve manipulating the 'pumpRun' variable, and I have determined that a 10-second duration suits my specific use case. Moreover, I will implement a fail-safe mechanism to cut the power supply to the pump if the sump's liquid level becomes excessively high, which could be attributed to an Esp32 malfunction. This will be a simple float switch.

Facilitating communication between the Esp and the Raspberry Pi will involve employing MQTT. This will allow the exchange of multiple messages between the two devices, enhancing their interaction and functionality.

1. ATO_Heart_Beat
2. ATO_Sump_Check
3. ATO_Container_Check
4. ATO_Refill

These will be sending either true or false as a msg.payload

These messages payloads will then be interpretaed within Node read to show a graphical indication of what is happening.

----

# Prof of concept


----

# Project development




































