/*
This code is an implementation of a state machine for a robot that searches for a bottle, captures it, and returns to its 
starting position. The robot receives commands from a Raspberry Pi via a serial connection. The code implements functions 
for turning right, turning left, going straight, searching, lowering the cage to capture the bottle, and raising the cage. 
The code is written in C++ for the Arduino platform and uses the AFMotor and Servo libraries to control the motors and servo. 
The state of the robot is determined by the "state" variable and its behavior is controlled by the switch case statement 
based on the command received from the Raspberry Pi.
*/


#include <AFMotor.h>
#include <Servo.h> 

// Functions Prototype 
void turnRight();
void turnLeft();
void goStraightAhead();
void searchInCurrentPlace();
void lowerCage();
void raiseCage();

// State Machine Implementation
char state; 
const int CAGE_UP = 0; // Mode 0 = CAGE_UP
const int CAGE_DOWN = 1; // Mode 1 = CAGE_DOWN
int cage_mode = CAGE_UP;
Servo servoLeft;
Servo servoRight;

// DC motor on M1,M2,M3,M4
AF_DCMotor motor1(1); // Front Left
AF_DCMotor motor2(2); // Front Right
AF_DCMotor motor3(3); // Rear Left
AF_DCMotor motor4(4); // Rear Right

// This code runs once upon boot. Used to initialize configurations.
void setup() {
 Serial.begin(9600);
 state = 'A'; // Initial State
 raiseCage();
 motor1.setSpeed(200);
 motor1.run(RELEASE);
 motor2.setSpeed(200);
 motor2.run(RELEASE);
 motor3.setSpeed(200);
 motor3.run(RELEASE);
 motor4.setSpeed(200);
 motor4.run(RELEASE);
}

void loop() {
  if (Serial.available()) { 
    int serialValue = Serial.read();
    Serial.println("Serial.read():");
    Serial.println(serialValue);
    switch (serialValue) {
      case 'R':
        if (cage_mode == CAGE_UP) { 
          turnRight();
          switch (state) {
            case 'A':
              state = 'A';
              break;
            case 'B':
              state = 'A';
              break;
            case 'C':
              state = 'A';
              break;
            case 'D':
              state = 'D';
              break;
            default:
              break;
          }
        }
        if (cage_mode == CAGE_DOWN) {
          searchInCurrentPlace(); 
        }
        break;
      case 'C':
        if (cage_mode == CAGE_UP) {
          goStraightAhead();
          switch (state) {
            case 'A':
              state = 'B';
              break;
            case 'B':
              state = 'C';
              break;
            case 'C':
              state = 'C';
              break;
            case 'D':
              state = 'D';
              break;
            default:
              break;
          }
        }
        if (cage_mode == CAGE_DOWN) {
          searchInCurrentPlace(); 
        }
        break;
      case 'L':
        if (cage_mode == CAGE_UP) {
          turnLeft();
          switch (state) {
            case 'A':
              state = 'A';
              break;
            case 'B':
              state = 'A';
              break;
            case 'C':
              state = 'A';
              break;
            case 'D':
              state = 'D';
              break;
            default:
              break;
          }
        }
        if (cage_mode == CAGE_DOWN) {
          searchInCurrentPlace();
        }
        break;
      case 'P':
        if (cage_mode == CAGE_UP) {
        }
        if (cage_mode == CAGE_DOWN) {
          // returnToPoster();
        }
        break;
      case 'S':
      default:
        if (cage_mode == CAGE_UP) {
          searchInCurrentPlace();
          switch (state) {
            case 'A':
              state = 'A';
              break;
            case 'B':
              state = 'A';
              break;
            case 'C':
              state = 'D';
              lowerCage();
              break;
            case 'D':
              state = 'D';
              break;
            default:
              break;
          }
        }
        if (cage_mode == CAGE_DOWN) {
          searchInCurrentPlace();
        }
        break;
    }
  } 
  else {
    if (cage_mode == CAGE_UP) {
      searchInCurrentPlace();
    }
    if (cage_mode == CAGE_DOWN) {
      searchInCurrentPlace();
    }
    Serial.println("Serial Not Availaible");
  }
  
// Setting the cage mode according to the state.
if (state == 'D') {
    cage_mode = CAGE_DOWN;
} else {
    cage_mode = CAGE_UP;
}


Serial.println("CurrentState=" + state);
delay(500);
}
void lowerCage() {
  servoLeft.attach(9);
  servoRight.attach(10);
  servoLeft.write(30);
  servoRight.write(150);
  servoLeft.detach();
  servoRight.detach();
}

void raiseCage() {
  servoLeft.attach(9);
  servoRight.attach(10);
  servoLeft.write(150);
  servoRight.write(30);
  servoLeft.detach();
  servoRight.detach();
}

void turnRight() { 
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  delay(2000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void turnLeft() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);
  delay(2000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void goStraightAhead() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(2000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void searchInCurrentPlace() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
  delay(2000);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}
