#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>

TinyGPSPlus gps;
SoftwareSerial ss(4, 3);

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  LoRa.begin(433E6);
}

void loop() {
  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      String message = "Latitude: " + String(gps.location.lat(), 6) + 
                       ", Longitude: " + String(gps.location.lng(), 6) + 
                       ", Altitude: " + String(gps.altitude.meters()) + 
                       ", Speed: " + String(gps.speed.kmph());
      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();
    }
  }
}
