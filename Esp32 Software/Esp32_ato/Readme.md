# ESP32 Auto Top Off

The sketch is taking on a good shape, and it will incorporate one sensor to monitor the sump return chamber, along with one sensor to monitor the ATO container. The pump in use will be a Tunzi ATO pump, powered by 12 volts. To activate the pump, I will utilize a Mosfet module instead of a relay, as Mosfets are solid-state components, eliminating the possibility of mechanical failure.

Adjusting the pump's runtime will involve manipulating the 'pumpRun' variable, and I have determined that a 10-second duration suits my specific use case. Moreover, I will implement a fail-safe mechanism to cut the power supply to the pump if the sump's liquid level becomes excessively high, which could be attributed to an Esp32 malfunction. This will be a simple float switch.

Facilitating communication between the Esp and the Raspberry Pi will involve employing MQTT. This will allow the exchange of multiple messages between the two devices, enhancing their interaction and functionality.

1. ATO_Heart_Beat
2. ATO_Sump_Check
3. ATO_Container_Check
4. ATO_Refill

These will be sending either true or false as a msg.payload

These messages then will be worked with in Node read to show a graphical indication of what is happening.
































