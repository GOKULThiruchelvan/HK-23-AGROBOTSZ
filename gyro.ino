#include <Wire.h>
#include <MPU6050.h>
#include <LoRa.h>

// MPU6050 object
MPU6050 mpu;

// LoRa settings
const int8_t NSS_PIN = 10;
const int8_t RESET_PIN = 9;
const int8_t DIO0_PIN = 8;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  mpu.initialize();

  if (!mpu.testConnection()) {
    Serial.println("MPU6050 connection failed");
    while (1);
  }

  // Initialize LoRa
  if (!LoRa.begin(915E6)) { // Change frequency to your region's LoRa frequency
    Serial.println("LoRa initialization failed");
    while (1);
  }
  LoRa.setPins(NSS_PIN, RESET_PIN, DIO0_PIN);
  LoRa.setTxPower(20);
}

void loop() {
  // Read accelerometer and gyroscope data
  int16_t ax, ay, az;
  int16_t gx, gy, gz;

  mpu.getAcceleration(&ax, &ay, &az);
  mpu.getRotation(&gx, &gy, &gz);

  // Prepare data to send
  String data = "Ax: " + String(ax) + " Ay: " + String(ay) + " Az: " + String(az) +
                " Gx: " + String(gx) + " Gy: " + String(gy) + " Gz: " + String(gz);

  // Send data via LoRa
  LoRa.beginPacket();
  LoRa.print(data);
  LoRa.endPacket();

  // Print data to Serial Monitor
  Serial.println(data);

  // Wait a bit before sending the next packet
  delay(1000);
}
