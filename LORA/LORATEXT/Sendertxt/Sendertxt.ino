#include <SPI.h>
#include <LoRa.h>

void setup() {
  Serial.begin(9600);
  while (!Serial);

  if (!LoRa.begin(433E6)) { // Use your module's frequency
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  Serial.println("LoRa Sender Initialized!");
}

void loop() {
  Serial.println("Sending packet...");
  LoRa.beginPacket();
  LoRa.print("GPS!");
  LoRa.endPacket();
  delay(1000); // Wait a second before sending the next packet
}