#include <AccelStepper.h>

// PIN MAPPINGS (RAMPS 1.6)
#define BASE_STEP     26; #define BASE_DIR     28; #define BASE_EN     24
#define SHOULDER_STEP 54; #define SHOULDER_DIR 55; #define SHOULDER_EN 38
#define ELBOW_STEP    60; #define ELBOW_DIR    61; #define ELBOW_EN    56
#define ROLL_STEP     46; #define ROLL_DIR     48; #define ROLL_EN     62
#define FAN_PIN 9

// Steps per degree based on your Gear Ratios (1/8 microstepping)
const float SPD_BASE     = 10.55; // (200*8*19)/360
const float SPD_SHOULDER = 11.11; // (200*8*20)/360
const float SPD_ELBOW    = 11.11; // (200*8*20)/360
const float SPD_ROLL     = 16.11; // (200*8*29)/360

AccelStepper base(AccelStepper::DRIVER, 26, 28);
AccelStepper shoulder(AccelStepper::DRIVER, 54, 55);
AccelStepper elbow(AccelStepper::DRIVER, 60, 61);
AccelStepper roll(AccelStepper::DRIVER, 46, 48);

void setup() {
  Serial.begin(115200);
  pinMode(FAN_PIN, OUTPUT); digitalWrite(FAN_PIN, HIGH);
  
  int pins[] = {24, 38, 56, 62};
  for(int p : pins) { pinMode(p, OUTPUT); digitalWrite(p, HIGH); } // Start Disabled

  base.setMaxSpeed(1500); base.setAcceleration(500);
  shoulder.setMaxSpeed(1500); shoulder.setAcceleration(300);
  elbow.setMaxSpeed(1500); elbow.setAcceleration(500);
  roll.setMaxSpeed(1000); roll.setAcceleration(500);
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n'); cmd.trim(); cmd.toLowerCase();

    if (cmd == "e") {
      int pins[] = {24, 38, 56, 62};
      for(int p : pins) digitalWrite(p, LOW);
      Serial.println("System Enabled");
    } 
    else if (cmd == "d") {
      int pins[] = {24, 38, 56, 62};
      for(int p : pins) digitalWrite(p, HIGH);
      Serial.println("System Disabled");
    }
    else if (cmd.startsWith("up")) {
      float angle = cmd.substring(3).toFloat();
      base.move(angle * SPD_BASE); shoulder.move(angle * SPD_SHOULDER);
      elbow.move(angle * SPD_ELBOW); roll.move(angle * SPD_ROLL);
      Serial.println("Moving UP simultaneously...");
    }
    else if (cmd.startsWith("down")) {
      float angle = cmd.substring(5).toFloat();
      base.move(-angle * SPD_BASE); shoulder.move(-angle * SPD_SHOULDER);
      elbow.move(-angle * SPD_ELBOW); roll.move(-angle * SPD_ROLL);
      Serial.println("Moving DOWN simultaneously...");
    }
  }
  base.run(); shoulder.run(); elbow.run(); roll.run();
}