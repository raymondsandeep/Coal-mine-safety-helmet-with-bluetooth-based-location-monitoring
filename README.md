# Coal-mine-safety-helmet-with-bluetooth-based-location-monitoring
An embedded system based helmet was made with BLE for location tracking and LoRa for the transmission of data in real time. The workers were ensured more safety with the incorporation of sensors, an automatic alert for emergencies, and a panic button.

Hardware Components: LoRa SX1278 Module ESP32 (Receiver & BLE Anchor Nodes) Arduino Mega (Transmitter - Helmet Unit) MQ2 Gas Sensor DHT11 Sensor MPU6050 Accelerometer Push Button Buzzer & LED

Software & Programming Languages: Arduino IDE(Embedded C / C++ ) Python flask (for backend) HTML (for dashboard)

Description: This project develops a Smart Safety Helmet for miners, integrating gas detection, motion sensing, and BLE-based location tracking to enhance underground safety. The helmet features a MQ2 sensor to monitor methane and carbon monoxide levels, a DHT11 sensor for temperature and humidity, and an MPU6050 sensor for motion detection. A push button allows miners to send emergency alerts. A LoRa module transmits real-time sensor data to a monitoring station, ensuring continuous safety monitoring. BLE-based tracking with ESP32 anchor nodes enables precise indoor localization of miners, aiding in rescue operations during emergencies. If hazardous conditions are detected, a buzzer and LED alert the miner and notify the control room. This system ensures rapid response in emergencies, improving safety and efficiency in mining operations.
