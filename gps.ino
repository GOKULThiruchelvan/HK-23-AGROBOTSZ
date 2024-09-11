#include <TinyGPS++.h>
#include <SoftwareSerial.h>


static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;


TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  Serial.begin(9600);  // Open serial monitor at 9600 baud
  ss.begin(GPSBaud);  

  Serial.println("GPS Module Testing");
}

void loop() {

  while (ss.available() > 0) {
    gps.encode(ss.read());
    if (gps.location.isUpdated()) {
      
      Serial.print("Latitude: ");
      Serial.println(gps.location.lat(), 6);
      Serial.print("Longitude: ");
      Serial.println(gps.location.lng(), 6);
      Serial.print("Altitude: ");
      Serial.println(gps.altitude.meters());
      Serial.print("Speed: ");
      Serial.println(gps.speed.kmph());
      Serial.println();
    }
  }
}
