#include <SPI.h>
#include <LoRa.h>

int Led = 3;
String inputString = "";        
boolean stringComplete = false; 

void setup() {
  pinMode(Led, OUTPUT);
  Serial.begin(9600);
  while (!Serial);

  Serial.println("LoRa Receiver");

  if (!LoRa.begin(433E6)) { //use (915E6) for LoRa Ra-02 915 MHz
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  inputString.reserve(200);
  digitalWrite(Led, HIGH);
  delay(100);
  digitalWrite(Led, LOW);
  delay(100);
  digitalWrite(Led, HIGH);
  delay(100);
  digitalWrite(Led, LOW);
  delay(100);
}

void loop() {
  serialEvent();
  if (stringComplete) {
    Serial.println(inputString);
    if (inputString=="Led On\n"){
      digitalWrite(Led, HIGH);
 
    }
    else if (inputString=="Led Blink\n"){
      digitalWrite(Led, HIGH);
      delay(500);
      digitalWrite(Led, LOW);
      delay(500);
      digitalWrite(Led, HIGH);
      delay(500);
      digitalWrite(Led, LOW);
      delay(500);
      digitalWrite(Led, HIGH);
      delay(500);
      digitalWrite(Led, LOW);
      delay(500);
      digitalWrite(Led, HIGH);
      delay(500);
      digitalWrite(Led, LOW);
      delay(500);
    }
    else if (inputString=="Led Off\n"){
      digitalWrite(Led, LOW);
    }
    inputString = "";
    stringComplete = false;
  } 
}

void serialEvent() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {

    while (LoRa.available()) {
      char inChar = (char)LoRa.read();
      inputString += inChar;

      if (inChar == '\n') {
        stringComplete = true;
      }
    }


  }
}
