#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEAdvertising.h>

#define WALL_BLE_MAC "f4:65:0b:4a:b5:1a"  // Replace with actual MAC
#define WALL_BLE_NAME "WallBLE1"           // Give a name to your wall BLE

BLEAdvertising* pAdvertising;

void setup() {
    Serial.begin(115200);
    Serial.println("Starting Wall BLE...");

    BLEDevice::init(WALL_BLE_NAME);
    BLEServer* pServer = BLEDevice::createServer();
    pAdvertising = BLEDevice::getAdvertising();

    // Set advertisement data
    BLEAdvertisementData advertisementData;
    advertisementData.setName(WALL_BLE_NAME);
    advertisementData.setManufacturerData(String("Closest BLE: ") + WALL_BLE_MAC);
    pAdvertising->setAdvertisementData(advertisementData);
    
    pAdvertising->start();
}

void loop() {
    Serial.println("Advertising Wall BLE...");
    delay(5000);
}
