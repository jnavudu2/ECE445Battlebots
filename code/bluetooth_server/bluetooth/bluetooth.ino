#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLESecurity.h>

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

union control_message {
    struct {
        uint8_t weapon_on;
        int8_t left_motor_speed;
        int8_t right_motor_speed;
    } fields;
    uint8_t data[3];
};

BLECharacteristic *pCharacteristic;

char receivedMessage[100];
char buf[100];

uint8_t active_connection = 0;
int8_t left_motor_speed = 0;
int8_t right_motor_speed = 0;
uint8_t weapon_on = 0;


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      Serial.println("Device connected");
    }

    void onDisconnect(BLEServer* pServer) {
      Serial.println("Device disconnected");
      BLEDevice::startAdvertising();
      Serial.println("Advertising connection");
    }
};

class MyCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic* pCharacteristic) {
      active_connection = 1;
      union control_message* msg = (union control_message*)pCharacteristic->getData();
      // perform a sanity check to make sure we're getting good values
      if (msg->fields.weapon_on <= 1) {
        left_motor_speed = msg->fields.left_motor_speed;
        right_motor_speed = msg->fields.right_motor_speed;
        weapon_on = msg->fields.weapon_on;
      }
      snprintf(receivedMessage, 100, "Left: %d, Right: %d, Weapon: %d", left_motor_speed, right_motor_speed, weapon_on);
      Serial.println(receivedMessage);
    }
    void onRead(BLECharacteristic* pCharacteristic) {
      snprintf(buf, 100, "You said: %s", receivedMessage);
      pCharacteristic->setValue(buf);
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE server setup");

  memset(receivedMessage, 100, 0);
  memset(buf, 100, 0);

  BLEDevice::init("ESP32-BLE");
  BLEServer* pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  BLEService* pService = pServer->createService(SERVICE_UUID);
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ |
                      BLECharacteristic::PROPERTY_WRITE |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );

  pCharacteristic->setCallbacks(new MyCallbacks());
  
  pCharacteristic->setAccessPermissions(ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE);

  BLESecurity *pSecurity = new BLESecurity();
  pSecurity->setAuthenticationMode(ESP_LE_AUTH_NO_BOND);
  pSecurity->setCapability(ESP_IO_CAP_NONE);
  pSecurity->setInitEncryptionKey(ESP_BLE_ENC_KEY_MASK | ESP_BLE_ID_KEY_MASK);

  pService->start();

  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("Characteristic defined and advertising connection");
}

void loop() {
  active_connection = 0;
  delay(2000);
  Serial.println(active_connection);
}
