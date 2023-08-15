# Version 1h explained

I will explain various functions only that I feel need further explanations.

![Screenshot 2023-08-15 174116](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/f1c12745-4850-4b67-9232-5c38ade40fa5)


This function is designed to reboot the ESP32 periodically, approximately every 28 days. Let's break down how it works step by step:

(millis() % restartTimer):

1. millis() returns the current value of the internal timer in milliseconds since the program started running.
restartTimer is the time interval I have set for rebooting, which is 28 days expressed in milliseconds.
The modulo operator % calculates the remainder when millis() is divided by restartTimer. This effectively calculates how much time has passed since the last complete cycle of restartTimer.


2. ((millis() % restartTimer) < 2000):

 This condition checks if the remainder obtained from the modulo operation is less than 2000 milliseconds (2 seconds).
The idea here is to check if the current time is within a short period before a full cycle of restartTimer completes. This is used to ensure that the code for saving to EEPROM and restarting is executed only when the desired reboot time is very close.

3. EEPROM.write(0, minTempThreshold) and EEPROM.write(1, maxTempThreshold):

If the condition above is met, meaning the current time is within the specified window before a full cycle of restartTimer, the function proceeds to save the values of minTempThreshold and maxTempThreshold to EEPROM.
These values are stored in EEPROM memory locations 0 and 1, respectively.

4. EEPROM.commit():

After writing the values to EEPROM, this function call commits the changes, ensuring they are saved in EEPROM memory.

5. delay(2000):

A delay of 2000 milliseconds (2 seconds) is introduced after saving the values to EEPROM.
This delay provides some time for the EEPROM write operation to complete before the reboot.

6. ESP.restart():

Finally, after the delay, the ESP32 is rebooted using the ESP.restart() function. This causes the microcontroller to start running the program from the beginning.



































