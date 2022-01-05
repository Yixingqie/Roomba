#include <HCSR04.h>
#include <Servo.h>

const int FRONT_THRESH = 25;
const int RIGHT_THRESH = 20;
const int LEFT_THRESH = 20;
const int DIAG_LEFT_THRESH = 25;
const int DIAG_RIGHT_THRESH = 25;

void goStraight();
void goLeft();
void goRight();

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
  pinMode(LED_BUILTIN, OUTPUT);
  leftServo.attach(LEFT_SERVO);
  rightServo.attach(RIGHT_SERVO);
  goStraight();
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);

}

void loop() {
  front = frontSensor.measureDistanceCm();
  delay(5);
  right = rightSensor.measureDistanceCm();
  delay(5);
  left = leftSensor.measureDistanceCm();
  delay(5);
  diagRight = diagRightSensor.measureDistanceCm();
  delay(5);
  diagLeft = diagLeftSensor.measureDistanceCm();
  delay(5);

  Serial.print("Front: ");
  Serial.println(front);
  Serial.print("Right: ");
  Serial.println(right);
  Serial.print("Left: ");
  Serial.println(left);
  Serial.print("DiagLeft: ");
  Serial.println(diagLeft);
  Serial.print("DiagRight: ");
  Serial.println(diagRight);
  Serial.println();
  if (checkDir(front, FRONT_THRESH) && checkDir(diagLeft, DIAG_LEFT_THRESH) && checkDir(diagRight, DIAG_RIGHT_THRESH)) {
    if (left > right) {
      goLeft();
    } else {
      goRight();
    }
    delay(50);
  } else if (checkDir(front, FRONT_THRESH) && (checkDir(left, LEFT_THRESH) || checkDir(diagLeft, DIAG_LEFT_THRESH))) {  //Left Corner
    goRight();
    delay(50);
  } else if (checkDir(front, FRONT_THRESH) && (checkDir(right, RIGHT_THRESH) || checkDir(diagRight, DIAG_RIGHT_THRESH))) {     //Right Corner
    goLeft();
    delay(50);
  } else if (checkDir(left, LEFT_THRESH) || checkDir(diagLeft, DIAG_LEFT_THRESH)) { //Hitting Left Wall
    goRight();
    delay(50);
  } else if (checkDir(right, RIGHT_THRESH) || checkDir(diagRight, DIAG_RIGHT_THRESH)) {         //Hitting Right Wall
    goLeft();
    delay(50);
  } else if (checkDir(front, FRONT_THRESH)) {           //Hitting front Wall
    if (random(0, 2)) {
      goLeft();
    } else {
      goRight();
    }
    delay(50);
  } else {             //contrinue straight path
    goStraight();
  }
  // delay(500);
}

void goStraight() {
  rightServo.write(80);
  leftServo.write(90);
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
  if (val <= thresh && val != -1) {
    return 1;
  }
  return 0;
}
