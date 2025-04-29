
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

BLECharacteristic* myCharacteristic;
BLEServer* myServer;

#define MY_SERVICE_UUID        "12345678-1234-5678-1234-56789abcdef0"
#define MY_CHARACTERISTIC_UUID "abcdefab-1234-5678-1234-56789abcdef0"

int myCounter = 0;
unsigned long myLastUpdate = 0;

void setup() {
  Serial.begin(115200);
  BLEDevice::init("XIAO-BLE");

  myServer = BLEDevice::createServer();
  BLEService *myService = myServer->createService(MY_SERVICE_UUID);

  myCharacteristic = myService->createCharacteristic(
                      MY_CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ
                    );

  myCharacteristic->setValue("Count: 0");
  myService->start();

  BLEAdvertising *myAdvertising = BLEDevice::getAdvertising();
  myAdvertising->addServiceUUID(MY_SERVICE_UUID);
  myAdvertising->start();
  Serial.println("BLE Started, waiting for connection...");
}

void loop() {
  if (millis() - myLastUpdate > 5000) {
    myLastUpdate = millis();
    myCounter++;
    String myMessage = "Count: " + String(myCounter);
    myCharacteristic->setValue(myMessage.c_str());
    Serial.println("Updated: " + myMessage);
  }
}

