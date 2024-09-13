#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// Create a TinyGPS++ object
TinyGPSPlus gps;

// Create a SoftwareSerial object for GPS communication
SoftwareSerial gpsSerial(4, 3);  // RX: Pin 4, TX: Pin 3

void setup() {
  Serial.begin(115200);     // Initialize Serial Monitor
  gpsSerial.begin(9600);    // Initialize SoftwareSerial for GPS (baud rate 9600)
  
  Serial.println("GPS Module is Starting...");
}

void loop() {
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
      Serial.println(gps.speed.kmph());
      delay(3000);  // Print speed in km/h
    }
  }
}