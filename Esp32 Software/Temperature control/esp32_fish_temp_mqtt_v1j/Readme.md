# Version 1j explained

I will break down the various functions to try and help with understanding them.

One thing to be aware off is if you are used to using the EEPROM.h for Arduino Board EEPROM storage the ESP32 version is different so take a look at https://github.com/espressif/arduino-esp32/tree/master to ensure you are fully coversant with its use. This had me at first also it does not work as expected with floating point numbers. Hence the latest version to resolve that issue.

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

This function gets run during the void setup() funtion and resets the globa variables.

![Screenshot 2023-08-15 192809](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/42a27b74-3b4d-456c-808e-0ba182ce0a63)

1. minTempThreshold = EEPROM.read(0);
2. maxTempThreshold = EEPROM.read(1);

   <ins> This read the EEPROM declared positions and resets the 2 required variables. </ins>

2. lastMinTempThreshold = minTempThreshold;
3. lastMaxTempThreshold = maxTempThreshold;   

   In summary, the purpose of this function is to retrieve the minimum and maximum temperature threshold values from EEPROM and store them in corresponding variables. Additionally, it keeps track of the previous values of these thresholds using lastMinTempThreshold and lastMaxTempThreshold variables. This could be useful for comparison or logging purposes, allowing the program to determine if the threshold values have been changed since the last time they were read from EEPROM.
----

# void connectToMqtt() 
This fuction get called from the void main()

![Screenshot 2023-08-20 212810](https://github.com/johnmholmes/Aquarium_Monitor_V2/assets/60571002/80cb28ff-a069-49a0-b9ed-95d0a7afa114)

This function is attempting to establish a connection to an MQTT broker using the MQTT client library, presumably on an ESP8266-based device (as indicated by the client name "ESP8266Client"). MQTT (Message Queuing Telemetry Transport) is a lightweight messaging protocol commonly used for communication between devices over a network. Let's break down the function step by step:

1. void connectToMqtt(): This line defines a function named connectToMqtt with a void return type, meaning it doesn't return any value.
2. while (!mqttClient.connected()) {: This line initiates a while loop that continues as long as the MQTT client (mqttClient) is not connected to the MQTT broker. In other words, it keeps looping until the connection is successfully established.
3. Serial.println("Attempting MQTT connection...");: This line sends a message to the serial port (often used for debugging purposes) indicating that an attempt to connect to the MQTT broker is being made.
4. if (mqttClient.connect("ESP8266Client")) {: Here, an attempt to connect to the MQTT broker is made using the connect method of the mqttClient object. The argument "ESP8266Client" is the client identifier that will be used when connecting to the broker.

   1. If the connection is successful (returns true), the code inside this block will execute.

   2. The code block within the if statement logs a message indicating a successful connection and then subscribes to two MQTT topics: "backupMaxSetting" and "backupMinSetting".
      } else {: If the connection attempt inside the if statement fails (returns false), the code within this else block will execute.
   
6. } else {: If the connection attempt inside the if statement fails (returns false), the code within this else block will execute.
7. Serial.print("Failed, rc=");: This line logs a message indicating that the connection attempt failed, followed by the return code (status) of the MQTT client's state.
8. Serial.print(mqttClient.state());: This line prints the numerical state of the MQTT client, which can help diagnose the reason for the failed connection attempt. Different numerical states represent different connection statuses.
9. Serial.println(" retrying in 5 seconds");: This line logs a message indicating that a retry will occur in 5 seconds.
10. delay(5000);: This line introduces a delay of 5 seconds using the delay function. It's a common practice to wait a short amount of time before attempting to connect again, to avoid overwhelming the MQTT broker with connection attempts.

The function will keep looping through the connection attempt until it successfully connects to the MQTT broker. Once connected, it subscribes to the specified topics for receiving messages from the broker. If the connection fails, it waits for 5 seconds before making another connection attempt. This process continues until a successful connection is established.





































