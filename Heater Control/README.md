#  Heater Control

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
