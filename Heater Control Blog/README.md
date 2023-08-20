# Heater Control Blog

# 18 August

After a few days of testing I can report that the backup heater module is working and is rebooting.

I have also been testing the new Main temperature module and that is working as expected. I now need to create a simple box for the unit and get it finally installed.

# 16 August

The final version is now complete the changes to the code to use a simpler mqtt library which does not need the complexity of the Asycnmqtt library I feel more comforatble with. Its only disadvantage is it boes not have the quality of service or the retain flag. It is possible to controll the temperature to a few decimal places, however this becomes a problem when saving and importing them from the EEPROM So for this setup I will be happy to accept a 1 degree Celisus difference.

# 14 August

The Tests are quite interesting and here is a graph showing the heat on / Off cycle, this shows the 200 watt heater is taking 1 hour to heat the tank.

The variations in the graph are caused by me trying different setting for the module to check that the new variables are working. This will be narrowed down tomorrow after a 24 hour period with the lates settings.

![Screenshot 2023-08-14 222519](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/7a960a98-2384-416c-a7c9-869a206b1195)
![Screenshot 2023-08-14 222716](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/7d8edb45-2398-44d4-bf57-19e16a674cfc)

I have also replaced the sketch and splite off the void loop into seperate function calls which helps make the code easier to understand and maintain. You can now set the min max vale at the start of the skecth. And it now reads the state of the GPIO pin 23 for sending the MQTT heater status. This fixed an issue with the way I had written the previous versions.

I have now added a folder to the Esp32 Software section that has the previous codes and a brief overview of the changes.

The final version of the backup is nearly complete and this has a reboot timer being added. This will all be shown in the next video the release is planed for Wednesday 16 August

----

# 10 August

The backup heater Esp32 module has now be setup and is going through testing stages the code is the same as the previous one , but with adjustments to the control temperatures be lower.


# 9 August

The stray reading are annoying though they maybe due to bad connections as when I move the pi one sensor reported 80 DEG C. I am tempeted to move all the ds18b20 sensors to a ESP32 as I have been reading up on the Node Red forum that this can cuse a flow stall for 2 or more seconds depending on how many sensors you have connected. By moving them to the Esp32 and using the MQTT to send the data leaves the Node Red not to stall. So far I have not seen any issues with the 1 sensor on the ESP32.

----

# 8 August

Spent today working on a Tkinter Gui to work with the temperature reading using also sqlite database this was taking reading from a crontab python file.
----

# 7 August

The day has been spent working on getting the database working, but more work will be needed. I now have a Python script running using a crontab 1 every hour adding the reading into a database called john.db. I need to look at the data type and the details I want to add to it. So far I have not been able to pull the data and add it to a graph as its pulling the information but in the wrong data type by the look of it. I will only be using this to record the display temperture every hour. This will then put into a graph to show 1 weeks worth of data which requires 168 reading.

----

# 6 August.

An interesting occurrence took place last night when one of my return pumps, which has been in operation for over four years, experienced a failure around midnight. Fortunately, this incident did not disrupt the tank significantly; it merely led to a minor decrease in the water level within the display. Interestingly, this event was reflected in the data recorded on the graphs and gauges, as the display temperature exhibited a difference of 1.5 degrees Celsius lower than that of the sump.

This occurrence serves to reinforce my inclination to implement a separate heater within the display in the near future. Such a precaution would act as a safeguard against potential reoccurrences of similar incidents, offering an added layer of protection and stability to the tank's environment.

----

# 5 August.

The tank will continue to rely on the DD Aquarium controller as its primary heater power source. However, I am considering incorporating a relay cutout as a precautionary measure in case the unit ever malfunctions and leaves the heaters running continuously. This additional feature is particularly relevant due to the presence of one heater within the setup that lacks an integrated thermostat. While this particular heater has performed reliably over the years, it's important to acknowledge that all aging equipment should be considered for replacement after a certain period of use.

-----

# 4 August.
Although I have presented several different ideas on the YouTube Channel, I am still experimenting with a few concepts. In Video 10, I demonstrated a node that consolidated readings from all three sensors into an array, resulting in a visually appealing outcome. However, after further experimentation, I am gradually reconsidering the initial method. This reconsideration stems from the potential to eliminate false readings during startup and after deploying changes to the flows.

Currently, I am undecided on whether to employ a GPIO pin on the Pi for controlling a solid-state relay to activate the emergency heater in the display, or if I should delegate this task to the ESP32.

The graphs and gauges have all been organized, marking the completion of that aspect of the controller. Progress is being made on the Raspberry Pi enclosure, which should be finished in the next few days. Once the enclosure is complete, the design will be finalized, and a video will be produced to showcase the entire project.
