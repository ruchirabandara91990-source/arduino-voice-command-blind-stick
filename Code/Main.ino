

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

// --- Pin Definitions ---
#define TRIG_PIN 2          // Ultrasonic Sensor Trigger
#define ECHO_PIN 3          // Ultrasonic Sensor Echo
#define PROXIMITY_PIN 4     // IR / Proximity Sensor (Stairs/Slope detection)
#define METAL_PIN 5         // Inductive Proximity / Metal Detector Sensor
#define EMERGENCY_LED 6     // Night-time Warning Light
#define SOIL_SENSOR_PIN A0  // Soil Moisture Sensor (Water detection)

#define DF_RX 10            // DFPlayer Mini RX (Connect to Arduino TX via 1k resistor)
#define DF_TX 11            // DFPlayer Mini TX

// --- Threshold Settings ---
const int OBSTACLE_DIST_CM = 100; // Alert if obstacle is within 100 cm (1 meter)
const int WATER_THRESHOLD = 600;  // Analog threshold for water detection (Adjust based on sensor)
const unsigned long ALERT_COOLDOWN = 3000; // 3 seconds delay between voice alerts

// --- Voice Track Numbers on SD Card (MP3 Folder) ---
const int TRACK_OBSTACLE = 1; // 0001.mp3 - "ඉදිරියෙන් බාධකයක් ඇත"
const int TRACK_STAIRS   = 2; // 0002.mp3 - "ඉදිරියෙන් පඩිපෙළක් හෝ බෑවුමක් ඇත"
const int TRACK_WATER    = 3; // 0003.mp3 - "ඉදිරියෙන් ජලය ඇත"
const int TRACK_METAL    = 4; // 0004.mp3 - "ඉදිරියෙන් ලෝහමය ද්‍රව්‍යයක් ඇත"

// --- Objects & Global Variables ---
SoftwareSerial mp3Serial(DF_RX, DF_TX);
DFRobotDFPlayerMini myDFPlayer;
unsigned long lastAlertTime = 0;

void setup() {
  Serial.begin(9600);
  mp3Serial.begin(9600);

  // Initialize Sensor & Output Pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(PROXIMITY_PIN, INPUT);
  pinMode(METAL_PIN, INPUT);
  pinMode(EMERGENCY_LED, OUTPUT);

  // Turn on night-time safety indicator light
  digitalWrite(EMERGENCY_LED, HIGH);

  Serial.println(F("Initializing Smart Blind Stick..."));

  // Initialize DFPlayer Mini
  if (!myDFPlayer.begin(mp3Serial)) {
    Serial.println(F("DFPlayer Mini error! Check wiring and SD card."));
    while (true); // Halt if audio module fails
  }
  
  myDFPlayer.volume(28); // Set volume (0 to 30)
  Serial.println(F("System Ready."));
}

void loop() {
  // 1. Check for Front Obstacles (Ultrasonic Sensor)
  long distance = getUltrasonicDistance();
  if (distance > 0 && distance <= OBSTACLE_DIST_CM) {
    triggerVoiceAlert(TRACK_OBSTACLE, "Obstacle Detected!");
  }

  // 2. Check for Stairs or Slopes (Proximity Sensor)
  // Assuming sensor goes HIGH when ground drops away (adjust LOW/HIGH per sensor type)
  if (digitalRead(PROXIMITY_PIN) == HIGH) {
    triggerVoiceAlert(TRACK_STAIRS, "Stairs or Slope Detected!");
  }

  // 3. Check for Water / Wet Surface (Soil Moisture Sensor)
  int moistureValue = analogRead(SOIL_SENSOR_PIN);
  if (moistureValue < WATER_THRESHOLD) { // Lower value usually means higher conductivity/water
    triggerVoiceAlert(TRACK_WATER, "Water Detected Ahead!");
  }

  // 4. Check for Metal Objects (Metal Sensor)
  // Assuming active LOW output when metal is detected
  if (digitalRead(METAL_PIN) == LOW) {
    triggerVoiceAlert(TRACK_METAL, "Metal Object Detected!");
  }

  // Blink Emergency Light gently for visibility
  handleEmergencyLight();
  
  delay(100); // Short stability delay
}

// --- Function to Measure Distance via Ultrasonic Sensor ---
long getUltrasonicDistance() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000); // 30ms timeout
  if (duration == 0) return -1;
  
  return (duration * 0.034) / 2; // Convert to centimeters
}

// --- Function to Play Sinhala Voice Alerts without Overlapping ---
void triggerVoiceAlert(int trackNumber, const char* debugMsg) {
  if (millis() - lastAlertTime >= ALERT_COOLDOWN) {
    Serial.println(debugMsg);
    myDFPlayer.play(trackNumber);
    lastAlertTime = millis();
  }
}

// --- Non-blocking Blink for Night-time Emergency LED ---
void handleEmergencyLight() {
  static unsigned long prevBlink = 0;
  static bool ledState = false;
  
  if (millis() - prevBlink >= 500) {
    ledState = !ledState;
    digitalWrite(EMERGENCY_LED, ledState);
    prevBlink = millis();
  }
}