# ESP32 Auto Top Off Module

The sketch is starting to be developed in a series of small steps. The plan is, it will incorporate one sensor to monitor the sump return chamber, along with one sensor to monitor the ATO container. The pump I use is a Tunzi ATO pump as this fits through  the container opening. This is powered by 12 volts power supply, which will also power the Esp32, I will be utilizing a Mosfet module instead of a relay, as Mosfets are solid-state components, eliminating the possibility of mechanical failure.

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

Initial work bench testing is proving very positive and can be seen working in video 16 in this case I am using jumper wires to simulate the water proof infra red sensors while I make up the circuit boards required for these device to work correctly.

----

# Project development

My goal is to be able to interact with the ATO Esp32 module, and if needed be I will be able to over ride the system if required from outside of my home network. The reason for this may be if while I am away from the home for an extended period the infra red sensors can get dirty from algea growth and not trigger the pump to run. In this case sernario I could then triggered this event from Node Red to run the pump. I do use a Esp32can to monitor the sump area of the system.

The Esp32 unit I am using is approxamitly £6 at the moment in the UK and the number of free GPIO pins makes this simple to accomplish, by adding some extra functions to the sketch and Node Red. I am also using a £4 break out board to make connecting devices more easily. The total cost for this unit will be around £15 less the pump and power supply.

While commercial units are available I feel the amount of control I can have out ways these systems. Also if you buy direct from China vai aliexpress the cost is brought down to aapprox £10.



































