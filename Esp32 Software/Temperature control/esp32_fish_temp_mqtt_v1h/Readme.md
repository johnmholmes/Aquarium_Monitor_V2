# Version 1h explained

I will break down the various functions to help with understanding them.

![Screenshot 2023-08-15 175711](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/1f83eca9-e75a-49c1-bc99-899fb3d0a70a)

1. Modulo Calculation and Timer Check:

  (millis() % restartTimer) calculates the remainder when the current millis() value is divided by restartTimer. This helps determine how much time has passed since the last full cycle of the restart interval.

  The condition (millis() % restartTimer) < 2000 checks whether the time remaining until the next restart is less than 2000 milliseconds (2 seconds). This ensures that the subsequent code runs when the restart is imminent.

2. Value Check and EEPROM Write:

 Inside the outer if block, the code checks if the values of minTempThreshold or maxTempThreshold have changed since the last time they were written to EEPROM (lastMinTempThreshold and lastMaxTempThreshold).

 If either of these values has changed, the code writes the new values to EEPROM memory locations 0 and 1. This is an optimization to avoid unnecessary writes to EEPROM when the values haven't changed.  

3. EEPROM Write and Variable Update:

  After writing the values to EEPROM, the EEPROM.commit() function is called to ensure the changes are stored in EEPROM.
  
  The lastMinTempThreshold and lastMaxTempThreshold variables are updated with the new values. This ensures that only changes in these variables trigger EEPROM writes on the next iteration.

 4. Reboot:

  After the EEPROM write operation and update of tracking variables, the ESP32 is rebooted using ESP.restart().


  In summary, this version of the function improves efficiency by writing to EEPROM only when there are actual changes in the values of minTempThreshold and maxTempThreshold. This reduces unnecessary wear on the EEPROM 
memory and optimizes the program's behavior. The function also ensures that the reboot is timed close to the desired interval while considering changes in the threshold values.

----

































