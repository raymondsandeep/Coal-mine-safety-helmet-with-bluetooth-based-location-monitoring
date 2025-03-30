#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi Credentials
const char* ssid = "your_wifi_ssid";
const char* password = "your_wifi_password";

// Flask Server URL
const char* serverUrl = "**********"; // Update with your Flask server IP

// LoRa Pin Definitions
const int csPin = 5;
const int resetPin = 14;
const int irqPin = 26;

// Function to send data to Flask server
void sendToServer(String jsonData) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
        Serial.print("Data Sent! Server Response: ");
        Serial.println(httpResponseCode);
    } else {
        Serial.print("Error Sending Data. HTTP Response: ");
        Serial.println(httpResponseCode);
    }

    http.end();
}

void setup() {
    Serial.begin(115200);
    while (!Serial);

    // Connect to WiFi
    Serial.println("Connecting to WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting...");
    }
    Serial.println("WiFi Connected!");

    Serial.println("Initializing LoRa...");
    LoRa.setPins(csPin, resetPin, irqPin);
    if (!LoRa.begin(433E6)) {
        Serial.println("Starting LoRa failed!");
        while (1);
    }

    Serial.println("LoRa Receiver Initialized");
}

void loop() {
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        String receivedData = "";

        // Read incoming data
        while (LoRa.available()) {
            receivedData += (char)LoRa.read();
        }

        Serial.print("Received: ");
        Serial.println(receivedData);
        // receivedData = "{\"type\":\"BLE\",\"wall_name\":\""+receivedData + "}";
        // sendToServer(receivedData);
        if (receivedData.startsWith("Wall")) {
            // BLE Data Format: BLE,WallName,MAC,RSSI
            int firstComma = receivedData.indexOf(',');
            int secondComma = receivedData.indexOf(',', firstComma + 1);
            int thirdComma = receivedData.indexOf(',', secondComma + 1);

            // if (firstComma != -1 && secondComma != -1 && thirdComma != -1) {
            //     String wallName = receivedData.substring(firstComma + 1, secondComma);
            //     String macAddress = receivedData.substring(secondComma + 1, thirdComma);
            //     int rssi = receivedData.substring(thirdComma + 1).toInt();

            //     Serial.println("BLE Data Received:");
            //     Serial.println("Wall BLE Name: " + wallName);
            //     Serial.println("Wall BLE MAC: " + macAddress);
            //     Serial.println("RSSI: " + String(rssi) + " dBm");

                // Create JSON payload
                String jsonData = "{\"type\":\"BLE\",\"wall_name\":\"" + String(receivedData) + "\"}";  
                sendToServer(jsonData);
            // } else {
            //     Serial.println("Error: Invalid BLE Data Format");
            // }
        } else {
            // Sensor Data Format: buttonState,motionDetected,temperature,humidity,mq2Value
            float sensorData[5];
            int index = 0;
            char *token = strtok((char *)receivedData.c_str(), ",");

            while (token != NULL && index < 5) {
                sensorData[index++] = atof(token);
                token = strtok(NULL, ",");
            }

            if (index == 5) {
                Serial.println("Sensor Data Received:");
                Serial.print("Emergency Button: ");
                Serial.println(sensorData[0] ? "Pressed" : "Not Pressed");
                Serial.print("Motion Detected: ");
                Serial.println(sensorData[1] ? "Yes" : "No");
                Serial.print("Temperature: ");
                Serial.print(sensorData[2]);
                Serial.println(" °C");
                Serial.print("Humidity: ");
                Serial.print(sensorData[3]);
                Serial.println(" %");
                Serial.print("MQ2 Gas Level: ");
                Serial.println(sensorData[4]);

                // Create JSON payload
                String jsonData = "{\"type\":\"sensor\",\"button_state\":" + String(sensorData[0]) +
                                  ",\"motion_detected\":" + String(sensorData[1]) +
                                  ",\"temperature\":" + String(sensorData[2]) +
                                  ",\"humidity\":" + String(sensorData[3]) +
                                  ",\"mq2_value\":" + String(sensorData[4]) + "}";

                sendToServer(jsonData);
            } else {
                Serial.println("Error: Incomplete Sensor Data");
            }
        }
    }
}
