# Version 1h explained

I will break down the various functions to try and help with understanding them.

One thing to be aware off is if you are used to using the EEPROM.h for Arduino Board EEPROM storage the ESP32 version is different so take a look at https://github.com/espressif/arduino-esp32/tree/master to ensure you are fully coversant with its use. This had me at first.

I have found that using the EEPROM is handy, however there is an issue storing floating point numbers. So with this in mind I am more than happy to settle for a 1 degrree Celsius variation in water temperature.

----

# Reboot And Store Min Max Values To EEPROM

![Screenshot 2023-08-15 175711](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/1f83eca9-e75a-49c1-bc99-899fb3d0a70a)

1. if ((millis() % restartTimer) < 2000) {

   <ins> Modulo Calculation and Timer Check:</ins>

   (millis() % restartTimer) calculates the remainder when the current millis() value is divided by restartTimer. This helps determine how much time has passed since the last full cycle of the restart interval.

   The condition (millis() % restartTimer) < 2000 checks whether the time remaining until the next restart is less than 2000 milliseconds (2 seconds). This ensures that the subsequent code runs when the restart is imminent.

2. if (minTempThreshold != lastMinTempThreshold || maxTempThreshold != lastMaxTempThreshold) {  

    <ins>Value Check and EEPROM Write:</ins>

    Inside the outer if block, the code checks if the values of minTempThreshold or maxTempThreshold have changed since the last time they were written to EEPROM (lastMinTempThreshold and lastMaxTempThreshold).

    If either of these values has changed, the code writes the new values to EEPROM memory locations 0 and 1. This is an optimization to avoid unnecessary writes to EEPROM when the values haven't changed.  

3. . EEPROM.write(0, minTempThreshold);
 
   . EEPROM.write(1, maxTempThreshold);
 
   . EEPROM.commit();
 
   . lastMinTempThreshold = minTempThreshold;
 
   . lastMaxTempThreshold = maxTempThreshold;

    <ins>EEPROM Write and Variable Update:</ins>

    After writing the values to EEPROM, the EEPROM.commit() function is called to ensure the changes are stored in EEPROM.
  
    The lastMinTempThreshold and lastMaxTempThreshold variables are updated with the new values. This ensures that only changes in these variables trigger EEPROM writes on the next iteration.

 4. ESP.restart();
   
    <ins>Reboot:</ins>

    After the EEPROM write operation and update of tracking variables, the ESP32 is rebooted using ESP.restart().


    In summary, this function improves efficiency by writing to EEPROM only when there are actual changes in the values of minTempThreshold and maxTempThreshold. This reduces unnecessary wear on the EEPROM 
memory and optimizes the program's behavior. The function also ensures that the reboot is timed close to the desired interval while considering changes in the threshold values.

----

# EEPROM Min Max Value Restore

This function get during the void setup() funtion and resets the globa variables.

![Screenshot 2023-08-15 192809](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/42a27b74-3b4d-456c-808e-0ba182ce0a63)

1. minTempThreshold = EEPROM.read(0);
2. maxTempThreshold = EEPROM.read(1);

   <ins> This read the EEPROM declared positions and resets the 2 required variables

2. lastMinTempThreshold = minTempThreshold;
3. lastMaxTempThreshold = maxTempThreshold;   

   In summary, the purpose of this function is to retrieve the minimum and maximum temperature threshold values from EEPROM and store them in corresponding variables. Additionally, it keeps track of the previous values of these thresholds using lastMinTempThreshold and lastMaxTempThreshold variables. This could be useful for comparison or logging purposes, allowing the program to determine if the threshold values have been changed since the last time they were read from EEPROM.
----
































