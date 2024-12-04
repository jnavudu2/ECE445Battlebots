#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLESecurity.h>
#include <math.h>

#define DEVICE_NAME         "ESP32-BLE"
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

#define PWM_FREQ                  50
#define FULL_REVERSE_DUTY_CYCLE   5
#define ZERO_DUTY_CYCLE           7.5
#define FULL_FORWARD_DUTY_CYCLE   10
#define LEFT_MOTOR_PIN            38
#define RIGHT_MOTOR_PIN           37
#define WEAPON_MOTOR_PIN          39
#define PWM_RESOLUTION            10
#define PWM_CHANNEL               0

union control_message {
    struct {
        int8_t weapon_motor_speed;
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
int8_t weapon_motor_speed = 0;
// uint8_t weapon_on = 0;

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
      left_motor_speed = -1 * msg->fields.left_motor_speed;
      right_motor_speed = msg->fields.right_motor_speed;
      weapon_motor_speed = msg->fields.weapon_motor_speed;
      snprintf(receivedMessage, 100, "Left: %d, Right: %d, Weapon: %d", left_motor_speed, right_motor_speed, weapon_motor_speed);
      Serial.println(receivedMessage);
    }
    void onRead(BLECharacteristic* pCharacteristic) {
      // snprintf(buf, 100, "You said: %s", receivedMessage);
      // pCharacteristic->setValue(buf);
    }
};

void setup() {
  Serial.begin(115200);
  Serial.println("Starting BLE server setup");

  memset(receivedMessage, 100, 0);
  memset(buf, 100, 0);

  BLEDevice::init(DEVICE_NAME);
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

  ledcAttach(LEFT_MOTOR_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(RIGHT_MOTOR_PIN, PWM_FREQ, PWM_RESOLUTION);
  ledcAttach(WEAPON_MOTOR_PIN, PWM_FREQ, PWM_RESOLUTION);

  delay(1000);

  connectMotor(LEFT_MOTOR_PIN);
  connectMotor(RIGHT_MOTOR_PIN);
  connectMotor(WEAPON_MOTOR_PIN);
}

void connectMotor(int pin) {
  Serial.println("Starting Motor calibration");

  ledcWrite(pin, 1024);
  delay(140);
  ledcWrite(pin, 0);
  delay(300);

  Serial.println("Motor calibration done");
}

int findDuty(int motorSpeed) {
  float speed;
  speed = (2.5 * motorSpeed) / 128;
  speed += 7.5;
  speed *= 1024 / 100;
  int duty = round(speed);
  return duty;
}

void loop() {
  // active_connection = 0;
  delay(50);
  // Serial.println(active_connection);
  ledcWrite(LEFT_MOTOR_PIN, findDuty(left_motor_speed));
  ledcWrite(RIGHT_MOTOR_PIN, findDuty(right_motor_speed));
  ledcWrite(WEAPON_MOTOR_PIN, findDuty(weapon_motor_speed));
}
