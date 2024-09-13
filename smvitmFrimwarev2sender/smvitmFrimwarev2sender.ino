#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

// Initialize GPS and MPU6050
SoftwareSerial gpsSerial(4, 3);  // RX: Pin 4, TX: Pin 3
TinyGPSPlus gps;
Adafruit_MPU6050 mpu;

// LoRa frequency
#define LORA_FREQUENCY 433E6

// Define touch sensor pin 
const int touchSensorPin = 7;  // Connect touch sensor to digital pin 7

void setup() {
  // Initialize Serial communication
  Serial.begin(115200);
  while (!Serial);

  // Initialize LoRa
  if (!LoRa.begin(LORA_FREQUENCY)) {  // Use your module's frequency
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Sender Initialized!");

  // Initialize MPU6050
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1);
  }

  Serial.println("MPU6050 Found!");

  // Setup MPU6050
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 Initialized.");

  // Initialize GPS
  gpsSerial.begin(9600);    // Initialize SoftwareSerial for GPS (baud rate 9600) 
  Serial.println("GPS Module is Starting...");

  // Initialize touch sensor
  pinMode(touchSensorPin, INPUT);
}

void loop() {
  // Read touch sensor state
  int touchState = digitalRead(touchSensorPin);

  // Only proceed if the touch sensor is touched
  if (touchState == HIGH) {
    // Print message indicating process is going to start
    Serial.println("SOS Triggered:Sendind Data Packets");

    // Read GPS data
    while (gpsSerial.available() > 0) {  // Check if data is available on SoftwareSerial
      gps.encode(gpsSerial.read());      // Decode GPS data
    }

    // Read MPU6050 data
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    // Prepare data to send over LoRa
    String dataToSend = "";

    // Add GPS data if available
    if (gps.location.isUpdated()) {  // If new GPS data is available
      dataToSend += "Lat: " + String(gps.location.lat(), 6) + ", ";
      dataToSend += "Lng: " + String(gps.location.lng(), 6) + ", ";
      dataToSend += "Sat: " + String(gps.satellites.value()) + ", ";
      dataToSend += "Alt: " + String(gps.altitude.meters()) + "m, ";
      dataToSend += "Speed: " + String(gps.speed.kmph()) + "km/h; ";
    } else {
      dataToSend += "GPS data ; ";
    }

    // Add accelerometer data
    dataToSend += "Accel X: " + String(accel.acceleration.x) + "m/s^2, ";
    dataToSend += "Accel Y: " + String(accel.acceleration.y) + "m/s^2, ";
    dataToSend += "Accel Z: " + String(accel.acceleration.z) + "m/s^2; ";

    // Add gyroscope data
    dataToSend += "Gyro X: " + String(gyro.gyro.x) + "rad/s, ";
    dataToSend += "Gyro Y: " + String(gyro.gyro.y) + "rad/s, ";
    dataToSend += "Gyro Z: " + String(gyro.gyro.z) + "rad/s; ";

    // Add temperature data
    dataToSend += "Temp: " + String(temp.temperature) + "°C;";

    // Send data over LoRa
    Serial.println("Sending packet: " + dataToSend);
    LoRa.beginPacket();
    LoRa.print(dataToSend);
    LoRa.endPacket();

    // Prevent flooding the loop by adding a short delay after sending
    delay(1000);  
  } else {
    Serial.println("Waiting For SOS Responce");
  }

  delay(500);  // Short delay to avoid overwhelming the loop
}
