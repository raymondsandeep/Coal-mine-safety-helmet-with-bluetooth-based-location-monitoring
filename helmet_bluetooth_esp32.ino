#include <BLEDevice.h>

#define SCAN_TIME 5  // Scan duration in seconds

// Define Wall BLE MAC addresses and names
#define WALL_BLE_1_MAC "f4:65:0b:4a:b5:1a"
#define WALL_BLE_1_NAME "WallBLE_1"
#define WALL_BLE_2_MAC "f4:65:0b:4a:88:c2"
#define WALL_BLE_2_NAME "WallBLE_2"

void setup() {
    Serial.begin(115200);      // Debugging Serial Monitor
    Serial2.begin(9600, SERIAL_8N1, 16, 17);  // UART to Arduino Mega (TX = 17, RX = 16)
    BLEDevice::init("");
}

void loop() {
    Serial.println("Scanning for Wall BLEs...");
    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setActiveScan(true);
    BLEScanResults foundDevices = *pBLEScan->start(SCAN_TIME, false);

    int bestRSSI = -999;
    String closestWallBLE = "";
    String closestWallName = "";

    for (int i = 0; i < foundDevices.getCount(); i++) {
        BLEAdvertisedDevice device = foundDevices.getDevice(i);
        String macAddress = device.getAddress().toString().c_str();
        String deviceName = device.getName().c_str();
        int rssi = device.getRSSI();

        Serial.printf("Found: Name: %s, MAC: %s, RSSI: %d dBm\n", 
                      deviceName.c_str(), macAddress.c_str(), rssi);

        // Check if the detected device is one of the Wall BLEs
        if ((macAddress == WALL_BLE_1_MAC || macAddress == WALL_BLE_2_MAC) ||
            (deviceName == WALL_BLE_1_NAME || deviceName == WALL_BLE_2_NAME)) {
            
            // Determine the closest Wall BLE
            if (rssi > bestRSSI) {  
                bestRSSI = rssi;
                closestWallBLE = macAddress;
                closestWallName = deviceName;
            }
        }
    }

    if (closestWallBLE != "") {
        Serial.printf("Closest Wall BLE: %s (MAC: %s) with RSSI: %d dBm\n", 
                      closestWallName.c_str(), closestWallBLE.c_str(), bestRSSI);

        // Standardized BLE Data Format for UART Communication
        String bleData = closestWallName + "," + closestWallBLE + "," + String(bestRSSI);
        Serial2.println(bleData); // Send BLE data to Arduino Mega
        Serial.println("Sent BLE Data to Arduino Mega: " + bleData);
    } else {
        Serial.println("No Wall BLE detected.");
    }

    Serial.println("Scan complete.\n");
    delay(5000);
}
