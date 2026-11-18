#include <SD.h>
#include <SPI.h>
#include <SoftwareSerial.h>
const int chipSelect = 4;
SoftwareSerial mySerial(9, 10); // RX, TX
void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  Serial.println("Card initialized.");
}
void loop() {
  if (mySerial.available()) {
    char command = mySerial.read();
    if (command == 'play song number 1') {
      playFile("001.wav");
    } else if (command == '2') {
      playFile("002.wav");
    } else if (command == '3') {
      playFile("003.wav");
    } else if (command == '2') {
      playFile("song2.wav");
    } else if (command == '2') {
      playFile("song2.wav");
    } else if (command == '2') {
      playFile("song2.wav");
    }
  }
}

void playFile(const char* filename) {
  File audioFile = SD.open(filename);
  if (audioFile) {
    Serial.print("Playing ");
    Serial.println(filename);
    // Code to read from file and output to speaker
    audioFile.close();
  } else {
    Serial.print("Error opening ");
    Serial.println(filename);
  }
}
