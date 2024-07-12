#include <Arduino.h>
#include <OCServo.h>

byte id = 0x02;
int goal = 0;
unsigned long lastPrint = 0;

OCServo servo(id, &Serial1);

void setup() {
  Serial.begin(38400);

  // Servo setup
  servo.begin();
  servo.setBaudRate(38400);

  // Basic settings for servo mode
  servo.setMode(OCS_SERVO);
  OCSResponse fullResponse = servo.setMaxTorque(400);
}


void loop() {
  if(servo.reachedGoal()) {     // If the servo reached the goal
    goal = 360 - goal;          // Change the goal
    servo.setGoalAngle(goal);
  }

  // Print all the data every second
  if(millis() > lastPrint + 1000) {
    Serial.print("ID: 0x");           Serial.println(servo.getID(), HEX);
    Serial.print("Baud rate: ");      Serial.println(servo.getBaudRate());
    Serial.print("Max torque: ");     Serial.println(servo.getMaxTorque());
    Serial.print("Mode: ");           Serial.println((servo.getMode() == OCS_SERVO) ? "SERVO" : "MOTOR");    
    Serial.print("Goal Angle: ");     Serial.println(servo.getGoalAngle());
    Serial.print("Goal Position: ");  Serial.println(servo.getGoalPosition());
    Serial.print("Operation Time: "); Serial.println(servo.getOperationTime());
    Serial.print("Response Delay: "); Serial.println(servo.getResponseDelay());
    Serial.print("Response level: "); Serial.println((servo.getResponseLevel() == OCS_RESPOND_READ_ONLY) ? "READ ONLY" : "ALL");   
    Serial.print("Min Angle: ");      Serial.println(servo.getMinAngle());
    Serial.print("Max Angle: ");      Serial.println(servo.getMaxAngle());
    Serial.print("Min Voltage: ");    Serial.println(servo.getMinVoltage());
    Serial.print("Max Voltage: ");    Serial.println(servo.getMaxVoltage());
    Serial.print("Operation Speed: ");Serial.println(servo.getOperationSpeed());
    lastPrint = millis();
  }
}
