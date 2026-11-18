#include <SoftwareSerial.h>

// Define Bluetooth module pins
#define BT_TX 10 // Bluetooth TX to Arduino RX (pin 10)
#define BT_RX 11 // Bluetooth RX to Arduino TX (pin 11)

SoftwareSerial BTSerial(BT_TX, BT_RX); // RX, TX

// Define relay control pins
#define RELAY1_PIN 2
#define RELAY2_PIN 3
#define RELAY3_PIN 4
#define RELAY4_PIN 5
#define RELAY5_PIN 6
#define RELAY6_PIN 7

void setup() {
  // Initialize relay pins as outputs
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);
  pinMode(RELAY3_PIN, OUTPUT);
  pinMode(RELAY4_PIN, OUTPUT);
  pinMode(RELAY5_PIN, OUTPUT);
  pinMode(RELAY6_PIN, OUTPUT);

  // Start serial communication with Bluetooth module
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available()) {
    char command = BTSerial.read();

    // Check which character is received and control relays accordingly
    switch (command) {
      case 'F':
        digitalWrite(RELAY1_PIN, HIGH); // Turn on relay 1
        delay(1000); // Keep relay on for 5 seconds
        digitalWrite(RELAY1_PIN, LOW); // Turn off relay 1
        break;
      case 'B':
        digitalWrite(RELAY2_PIN, HIGH); // Turn on relay 2
        delay(1000); // Keep relay on for 5 seconds
        digitalWrite(RELAY2_PIN, LOW); // Turn off relay 2
        break;
      case 'c':
        digitalWrite(RELAY3_PIN, HIGH); // Turn on relay 3
        delay(1000); // Keep relay on for 5 seconds
        digitalWrite(RELAY3_PIN, LOW); // Turn off relay 3
        break;
      case 'd':
        digitalWrite(RELAY4_PIN, HIGH); // Turn on relay 4
        delay(1000); // Keep relay on for 5 seconds
        digitalWrite(RELAY4_PIN, LOW); // Turn off relay 4
        break;
      case 'e':
        digitalWrite(RELAY5_PIN, HIGH); // Turn on relay 5
        delay(1000); // Keep relay on for 5 seconds
        digitalWrite(RELAY5_PIN, LOW); // Turn off relay 5
        break;
      case 'f':
        digitalWrite(RELAY6_PIN, HIGH); // Turn on relay 6
        delay(1000); // Keep relay on for 5 seconds
        digitalWrite(RELAY6_PIN, LOW); // Turn off relay 6
        break;
      default:
        // Invalid command received
        break;
    }
  }
}
