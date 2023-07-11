#include <Arduino.h>
#include "config.h"

#define ROTATIONS 10 // Number of rotations for the calibration
#define MOTOR_TO_CALIBRATE 0 // Select motor to calibrate. 0-3 for "MOTOR1, MOTOR2, MOTOR3, MOTOR4"

// Motor pin definitions
int stepPins[4] = {MOTOR1_IN_A, MOTOR2_IN_A, MOTOR3_IN_A, MOTOR4_IN_A};
int dirPins[4] = {MOTOR1_IN_B, MOTOR2_IN_B, MOTOR3_IN_B, MOTOR4_IN_B};

// Calibration status and pulse count
bool calibrating = false;
int pulses = 0;

void setup()
{
    Serial.begin(9600);
    while (!Serial) {
    }
    pinMode(stepPins[MOTOR_TO_CALIBRATE], OUTPUT);
    pinMode(dirPins[MOTOR_TO_CALIBRATE], OUTPUT);

    Serial.println("Type 'calibrate' and press enter to start calibrating the motor.");
    Serial.println("Type 'stop' and press enter to stop calibrating the motor.");
}

void loop()
{
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');
        command.trim();
        if (command == "calibrate") {
            startCalibration();
        }
        else if (command == "stop") {
            stopCalibration();
        }
    }
    spinMotor();
}

void startCalibration() {
    calibrating = true;
    pulses = 0;
}

void stopCalibration() {
    calibrating = false;
    Serial.print("Motor ");
    Serial.print(MOTOR_TO_CALIBRATE+1);
    Serial.print(" Pulses needed for a full rotation: ");
    Serial.println(pulses / ROTATIONS);
}

void spinMotor()
{
    if (calibrating) {
        digitalWrite(dirPins[MOTOR_TO_CALIBRATE], HIGH);  // Assuming HIGH is the forward direction
        digitalWrite(stepPins[MOTOR_TO_CALIBRATE], HIGH);
        delayMicroseconds(500);  // Adjust this delay to change motor speed
        digitalWrite(stepPins[MOTOR_TO_CALIBRATE], LOW);
        delayMicroseconds(500);  // The total delay should match the speed your stepper motor is rated for
        pulses++;
    }
}
