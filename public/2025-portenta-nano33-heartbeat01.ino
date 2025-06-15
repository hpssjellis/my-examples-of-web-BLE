#include <ArduinoBLE.h>

// Reusing your UUID for the Heart Rate Service and Characteristic
BLEService myHeartRateService("19b10000-e8f2-537e-4f6c-d104768a1214");

// Simulated Heart Rate Measurement characteristic (read + notify)
BLEByteCharacteristic myHeartRateChar("19b10000-e8f2-537e-4f6c-d104768a1214", BLERead | BLENotify);

unsigned long myLastUpdate = 0;
int myHeartRate = 75;

void setup() {
  Serial.begin(9600);
  // while (!Serial);  // Optional: wait for Serial

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.setLocalName("HRMonitor-Jer");
  BLE.setAdvertisedService(myHeartRateService);

  myHeartRateService.addCharacteristic(myHeartRateChar);
  BLE.addService(myHeartRateService);

  myHeartRateChar.writeValue(0);  // Initialize with 0 BPM

  BLE.advertise();
  Serial.println("BLE Heart Rate Monitor ready");
}

void loop() {
  BLEDevice central = BLE.central();

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());

    while (central.connected()) {
      unsigned long myNow = millis();
      if (myNow - myLastUpdate > 1000) {
        myLastUpdate = myNow;

        myHeartRate += random(-3, 4);  // Simulate some variation
        myHeartRate = constrain(myHeartRate, 60, 100);

        myHeartRateChar.writeValue((byte)myHeartRate);

        Serial.print("Sending Heart Rate: ");
        Serial.println(myHeartRate);
      }
    }

    Serial.print("Disconnected from central: ");
    Serial.println(central.address());
  }
}

