#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <BLEServer.h>

#include <Wire.h>
#include <AHTxx.h>

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define GPIO4 4
#define GPIO5 5

#define DEVICE_NAME "air sensor"

float ahtValue;

AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);
BLECharacteristic *pCharacteristic = nullptr;

void printStatus() {
  switch (aht10.getStatus()) {
    case AHTXX_NO_ERROR:
      Serial.println(F("no error"));
      break;

    case AHTXX_BUSY_ERROR:
      Serial.println(F("sensor busy, increase polling time"));
      break;

    case AHTXX_ACK_ERROR:
      Serial.println(F("sensor didn't return ACK, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_DATA_ERROR:
      Serial.println(F("received data smaller than expected, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
      break;

    case AHTXX_CRC8_ERROR:
      Serial.println(F("computed CRC8 not match received CRC8, this feature supported only by AHT2x sensors"));
      break;

    default:
      Serial.println(F("unknown status"));
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  Serial.println("Starting BLE work!");

  BLEDevice::init(DEVICE_NAME);
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

  pCharacteristic->setValue("Hello World says Neil");
  pService->start();
  // BLEAdvertising *pAdvertising = pServer->getAdvertising();  // this still is working for backward compatibility
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  BLEAdvertisementData advertiseData;
  advertiseData.setServiceData(BLEUUID(CHARACTERISTIC_UUID), "foo");
  pAdvertising->setAdvertisementData(advertiseData);
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined! Now you can read it in your phone!");

  while (!aht10.begin(GPIO4, GPIO5)) {
    Serial.println(F("AHT1x not connected or fail to load calibration coefficient"));
    delay(5000);
  }

  Serial.println(F("AHT10 OK"));
}

void loop() {
  ahtValue = aht10.readTemperature();
  Serial.print(F("Temperature: "));

  if (ahtValue != AHTXX_ERROR) {
    Serial.println(ahtValue);
    std::string s = String(ahtValue, 2).c_str();
    pCharacteristic->setValue(s);
  } else {
    printStatus();
  }

  ahtValue = aht10.readHumidity(AHTXX_USE_READ_DATA);

  Serial.print(F("Humidity...: "));

  if (ahtValue != AHTXX_ERROR) {
    Serial.println(ahtValue);
  } else {
    printStatus();
  }

  delay(8000);
}
