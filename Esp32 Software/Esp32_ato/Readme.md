# ESP32 Auto Top Off

The sketch is taking good shape and it will have 2 sensors monitoring the sump return chamber and one monitoring the Ato container. The pump will be a Tunzi ATO pump and will be supplied via 12 volts. To be able to turn on the pump I will be using an Mosfet module rather than a relay as these are soldi state so no moving parts to fail. The Esp will communicate to the Raspbeery pi via Mqtt. There will be a number of messages being sent.

1. ATO_Heart_Beat
2. ATO_Sump_Check
3. ATO_Container_Check
4. ATO_Refill

These will be sending either true or false as a msg.payload

These messages then will be worked with in Node read to show a graphical indication of what is happening.






























