#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <MPU6050.h>
#include <DHT.h>

// Pin Definitions
const int buttonPin = 4;
const int csPin = 53;
const int resetPin = 9;
const int irqPin = 2;
const int mq2Pin = A0;
const int dhtPin = 7;

MPU6050 mpu;
DHT dht(dhtPin, DHT11);

int buttonState = 0;
int motionDetected = 0;
float humidity = 0.0;
float temperature = 0.0;
int mq2Value = 0;

void setup() {
    Serial.begin(9600);      // Serial Monitor
    Serial1.begin(9600);     // UART1 for ESP32 (TX1 = Pin 18, RX1 = Pin 19)

    Wire.begin();
    mpu.initialize();
    dht.begin();
    pinMode(buttonPin, INPUT_PULLUP);

    LoRa.setPins(csPin, resetPin, irqPin);
    if (!LoRa.begin(433E6)) {
        Serial.println("LoRa Initialization Failed!");
        while (1);
    }
    Serial.println("LoRa & UART Communication Initialized");
}

void loop() {
    // Read button state
    buttonState = digitalRead(buttonPin) == LOW ? 1 : 0;
    
    // Read accelerometer data from MPU6050
    int16_t ax, ay, az;
    mpu.getAcceleration(&ax, &ay, &az);
    motionDetected = (abs(ax) > 16000 || abs(ay) > 16000 || abs(az) > 20000) ? 1 : 0;

    // Read sensor values
    humidity = dht.readHumidity();
    temperature = dht.readTemperature();
    mq2Value = analogRead(mq2Pin);

    // Prepare sensor data string
    String sensorData = String(buttonState) + "," + String(motionDetected) + "," + 
                        String(temperature, 2) + "," + String(humidity, 2) + "," + String(mq2Value);

    // Send sensor data over LoRa
    LoRa.beginPacket();
    LoRa.print(sensorData);
    LoRa.endPacket();
    
    Serial.print("Sent Sensor Data via LoRa: ");
    Serial.println(sensorData);

    // Check for incoming BLE data from ESP32
    if (Serial1.available()) {  
        delay(100);  // Allow buffer to fill
        String bleData = Serial1.readStringUntil('\n');  
        bleData.trim(); // Remove unwanted spaces or newlines

        Serial.println("Raw BLE Data from ESP32: " + bleData);

        if (bleData.length() > 0) {
            // Parse received data (Wall Name, MAC, RSSI)
            int firstComma = bleData.indexOf(',');
            int secondComma = bleData.indexOf(',', firstComma + 1);

            if (firstComma != -1 && secondComma != -1) {
                String wallName = bleData.substring(0, firstComma);
                String macAddress = bleData.substring(firstComma + 1, secondComma);
                int rssi = bleData.substring(secondComma + 1).toInt();

                Serial.println("Parsed BLE Data:");
                Serial.println("Wall BLE Name: " + wallName);
                Serial.println("Wall BLE MAC: " + macAddress);
                Serial.println("RSSI: " + String(rssi) + " dBm");

                //Prepare BLE data string for LoRa (Maintaining same format)
                String bleLoRaData = wallName + "," + macAddress + "," + String(rssi);
                LoRa.beginPacket();
                LoRa.print(bleLoRaData);
                LoRa.endPacket();

                Serial.println("Sent BLE Data via LoRa: " + bleLoRaData);
            } else {
                Serial.println("BLE Data Parsing Error!");
            }
        } else {
            Serial.println("No valid BLE data received!");
        }
    }

    delay(2000);
}
