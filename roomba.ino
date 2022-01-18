#include <HCSR04.h>
#include <Servo.h>

const int FRONT_THRESH = 20;
const int RIGHT_THRESH = 10;
const int LEFT_THRESH = 10;
const int DIAG_LEFT_THRESH = 20;
const int DIAG_RIGHT_THRESH = 20;

const int SLOWDOWN = 255;
const int MAX_SPEED = 150;
const int OFF = 0;

const int ADJUST_PERIOD = 10;

void goStraight();
void goLeft();
void goRight();

int goneLeft, goneRight = 0;
int toggle = 0;

int checkDir(int val, int thresh);

const int LEFT_SERVO = 3;
const int RIGHT_SERVO = 2;
const int MOTOR = 13;

int front, left, right, diagLeft, diagRight = 0;

Servo leftServo;
Servo rightServo;
UltraSonicDistanceSensor frontSensor(8, 7);
UltraSonicDistanceSensor leftSensor(12, 11);
UltraSonicDistanceSensor rightSensor(6, 5);
UltraSonicDistanceSensor diagLeftSensor(A2, A1);
UltraSonicDistanceSensor diagRightSensor(4, A0);

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR, OUTPUT);
  leftServo.attach(LEFT_SERVO);
  rightServo.attach(RIGHT_SERVO);
}

void loop() {
  front = frontSensor.measureDistanceCm();
  delay(1);
  right = rightSensor.measureDistanceCm();
  delay(1);
  left = leftSensor.measureDistanceCm();
  delay(1);
  diagRight = diagRightSensor.measureDistanceCm();
  delay(1);
  diagLeft = diagLeftSensor.measureDistanceCm();
  delay(1);

  //  Serial.print("Front: ");
  //  Serial.println(front);
  //  Serial.print("Right: ");
  //  Serial.println(right);
  //  Serial.print("Left: ");
  //  Serial.println(left);
  //  Serial.print("DiagLeft: ");
  //  Serial.println(diagLeft);
  //  Serial.print("DiagRight: ");
  //  Serial.println(diagRight);
  //  Serial.println();
  int turnLeft = 0;
  int turnRight = 0;
  if (checkDir(front, FRONT_THRESH)) {
    if ((left + diagLeft) > (right + diagRight)) { //Front sensor
      turnLeft++;
    } else {
      turnRight++;
    }
  }
  if (checkDir(diagLeft, DIAG_LEFT_THRESH)) {  //Diag Left Sensor
    turnRight++;
  }
  if (checkDir(diagRight, DIAG_RIGHT_THRESH)) {     //Diag Right Corner
    turnLeft++;
  }
  if (checkDir(left, LEFT_THRESH)) { //Left Sensor
    turnRight++;
  }
  if (checkDir(right, RIGHT_THRESH)) {         //Right Sensor
    turnLeft++;
  }
  if (!turnLeft && !turnRight || turnLeft == turnRight) {            //contrinue straight path
    goStraight();
    //wiggle();
    goneLeft = 0;
    goneRight = 0;
    analogWrite(MOTOR, MAX_SPEED);
  } else if (turnRight > turnLeft && !goneRight) {
    analogWrite(MOTOR, OFF);
    goRight();
    delay(ADJUST_PERIOD);
    analogWrite(MOTOR, MAX_SPEED);
    goneRight = 1;
    goneLeft = 0;
  } else if (turnLeft > turnRight && !goneLeft) {
    analogWrite(MOTOR, OFF);
    goLeft();
    delay(ADJUST_PERIOD);
    analogWrite(MOTOR, MAX_SPEED);
    goneLeft = 1;
    goneRight = 0;
  }
}

void goStraight() {
  rightServo.write(80);
  leftServo.write(90);
}

void wiggle() {
  if (toggle) {
    rightServo.write(180);
    leftServo.write(180);
  } else {
    rightServo.write(0);
    leftServo.write(0);
  }
  delay(500);
  toggle = !toggle;
}

void goLeft() {
  rightServo.write(180);
  leftServo.write(180);
}

void goRight() {
  rightServo.write(0);
  leftServo.write(0);
}

int checkDir(int val, int thresh) {
  if (val <= thresh) {
    return 1;
  } else {
    return 0;
  }
}
