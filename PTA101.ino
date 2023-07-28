// this code is for the robotics competition which is exclusive for elex department 
// this code won the champion for the fastest category
// PTA 101 which stands for PassTheAdversities 101 - 10 members-1 primary goal 

#include <Stepper.h>
#include <AFMotor.h>
#include <Servo.h>

#define STEPS 100
Stepper stepper(STEPS, 22, 23, 24, 25);

int servoXnum = 10;
int rightservo = 9;
int input;
int angle = 90;
int rotation = 0;

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);
Servo servoX;



int pos = 0;    // variable to store the servo position

void setup() {
  stepper.setSpeed(255);
  servoX.attach(servoXnum);

  motor1.setSpeed(200);
  motor2.setSpeed(200);
  motor3.setSpeed(200);
  motor4.setSpeed(200);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

  Serial1.begin(9600);
}

void loop() {
  uint8_t numspeed = 255  ;

  if (Serial.available() <= 0) {}
  if (Serial1.available() > 0) {
    input = Serial1.read();
  }

  if (input == '0') {
    motor1.setSpeed(numspeed);
    motor2.setSpeed(numspeed);
    motor3.setSpeed(numspeed);
    motor4.setSpeed(numspeed);
    motor1.run(RELEASE);
    motor2.run(RELEASE);
    motor3.run(RELEASE);
    motor4.run(RELEASE);
    stepper.step(0);
  }

  if (input == '1') {
    motor1.setSpeed(numspeed);
    motor2.setSpeed(numspeed);
    motor3.setSpeed(numspeed);
    motor4.setSpeed(numspeed);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
  }


  if (input == '2') {
    motor1.setSpeed(numspeed);
    motor2.setSpeed(numspeed);
    motor3.setSpeed(numspeed);
    motor4.setSpeed(numspeed);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
  }


  if (input == '3') {
    motor1.setSpeed(numspeed);
    motor2.setSpeed(numspeed);
    motor3.setSpeed(numspeed);
    motor4.setSpeed(numspeed);
    motor1.run(BACKWARD);
    motor2.run(BACKWARD);
    motor3.run(FORWARD);
    motor4.run(FORWARD);
  }

  if (input == '4') {
    motor1.setSpeed(numspeed);
    motor2.setSpeed(numspeed);
    motor3.setSpeed(numspeed);
    motor4.setSpeed(numspeed);
    motor1.run(FORWARD);
    motor2.run(FORWARD);
    motor3.run(BACKWARD);
    motor4.run(BACKWARD);
  }

  if (input == '5') {  
    stepper.step(50);
  }
  if (input == '6') {
    stepper.step(-50);
  }
}
