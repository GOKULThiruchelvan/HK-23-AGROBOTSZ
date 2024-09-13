#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

SoftwareSerial gpsSerial(4, 3);  // RX: Pin 4, TX: Pin 3
TinyGPSPlus gps;

Adafruit_MPU6050 mpu;

void setup() {
  // Initialize the serial communication
  Serial.begin(115200);
  while (!Serial) {
    delay(10);  // Wait until the serial monitor is ready
  }

  // Initialize I2C communication
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");

  // Setup MPU6050 (you can change scale and range)
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 Initialized.");

  // Initialize SoftwareSerial for GPS (baud rate 9600)
  gpsSerial.begin(9600);
  Serial.println("GPS Module is Starting...");
}

void loop() {
  // Read GPS data
  while (gpsSerial.available() > 0) {  // Check if data is available on SoftwareSerial
    gps.encode(gpsSerial.read());      // Decode GPS data
    
    if (gps.location.isUpdated()) {  // If new GPS data is available
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);  // Print latitude
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);  // Print longitude
      Serial.print("Satellites: ");
      Serial.println(gps.satellites.value());  // Print number of satellites
      Serial.print("Altitude (meters): ");
      Serial.println(gps.altitude.meters());  // Print altitude in meters
      Serial.print("Speed (km/h): ");
      Serial.println(gps.speed.kmph());  // Print speed in km/h
    }
  }

  // Read MPU6050 data
  sensors_event_t accel, gyro, temp;
  mpu.getEvent(&accel, &gyro, &temp);

  // Print accelerometer values
  Serial.print("Accel X: ");
  Serial.print(accel.acceleration.x);
  Serial.print(" m/s^2, ");
  Serial.print("Accel Y: ");
  Serial.print(accel.acceleration.y);
  Serial.print(" m/s^2, ");
  Serial.print("Accel Z: ");
  Serial.print(accel.acceleration.z);
  Serial.println(" m/s^2");

  // Print gyroscope values
  Serial.print("Gyro X: ");
  Serial.print(gyro.gyro.x);
  Serial.print(" rad/s, ");
  Serial.print("Gyro Y: ");
  Serial.print(gyro.gyro.y);
  Serial.print(" rad/s, ");
  Serial.print("Gyro Z: ");
  Serial.print(gyro.gyro.z);
  Serial.println(" rad/s");

  // Print temperature values (optional)
  Serial.print("Temperature: ");
  Serial.print(temp.temperature);
  Serial.println(" °C");

  // Add a delay to prevent flooding the serial output
  delay(500);
}
