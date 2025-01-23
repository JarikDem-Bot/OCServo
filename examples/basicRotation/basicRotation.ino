#include <Arduino.h>
#include <OCServo.h>

byte id = 0x02;

OCServo servo(id, &Serial1);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(38400);

  // Servo setup
  servo.begin();
  servo.setBaudRate(38400);

  // Basic settings for servo mode
  servo.setMode(OCS_SERVO);
  servo.setMaxTorque(200);
}


void loop() {
  // Move the servo to 360 degrees for 3 seconds
  servo.setGoalPosition(360, 3000);
  delay(3000);

  // Move the servo to 0 degrees for 3 seconds
  servo.setGoalPosition(0, 3000);
  delay(3000);
}
