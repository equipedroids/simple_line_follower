#include <FalconRobot.h>

FalconRobotLineSensor left(A2);
FalconRobotLineSensor right(A3);
FalconRobotMotors motors(5, 7, 6, 8);

#define SENSOR_VALUE_GROUND 900
#define SENSOR_VALUE_LINE 1010

#define MOTOR_MAX_SPEED 90
#define MOTOR_MIN_SPEED -10

int leftValue;  // variable to store the left sensor value
int rightValue;  // variable to store the right sensor value

int leftSpeed;   // variable used to store the leftMotor speed
int rightSpeed;  // variable used to store the rightMotor speed

void setup() {
  Serial.begin(9600);
  Serial.println("Its starts with");
  delay(4000);
  Serial.println("one thing");
}

void loop() {
  readSensors();

  leftSpeed = getLeftMotorSpeed();
  rightSpeed = getRightMotorSpeed();

  if(!isInLine()){
    // if loses the line, searches in the side that sensor was triggered last
    searchLine();
  }else{
    // just apply the speed to motors
    runMotors();
  }
  
  printSensorValues();
  delay(0);  // add a delay to decrease sensitivity.
}

void readSensors(){
  leftValue = left.read();
  rightValue = right.read();
}

int getLeftMotorSpeed(){
  // map speed based on sensor values
  return map(rightValue, SENSOR_VALUE_GROUND, SENSOR_VALUE_LINE, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
}

int getRightMotorSpeed(){
  // map speed based on sensor values
  return map(leftValue, SENSOR_VALUE_GROUND, SENSOR_VALUE_LINE, MOTOR_MIN_SPEED, MOTOR_MAX_SPEED);
}

bool isInLine(){
  return true;
  //return rightValue > SENSOR_VALUE_GROUND-10 || leftValue > SENSOR_VALUE_GROUND-10;
}

void runMotors(){
  if(leftSpeed < 0){
    motors.leftDrive(leftSpeed*-1, BACKWARD);
  }else{
    motors.leftDrive(leftSpeed, FORWARD);
  }
  if(rightSpeed < 0){
    motors.rightDrive(rightSpeed*-1, BACKWARD);
  }else{
    motors.rightDrive(rightSpeed, FORWARD);
  }
}

void stopMotors(){
  motors.leftDrive(0, FORWARD);
  motors.rightDrive(0, FORWARD);
}

void searchLine(){
  if(shouldSearchLineAtRight()){
    searchLineAtRight();
  }else{
    searchLineAtLeft();
  }  
  while(!isInLine()){
    readSensors();
  }
  stopMotors();
}

bool shouldSearchLineAtRight(){
  if(rightValue > leftValue){
    return true;
  }else{
    return false;
  }
}

void searchLineAtRight(){
  Serial.println("searching right");
  motors.leftDrive(40, FORWARD);
  motors.rightDrive(0, FORWARD);
}

void searchLineAtLeft(){
  Serial.println("searching left");
  motors.leftDrive(0, FORWARD);
  motors.rightDrive(40, FORWARD);
}

void printSensorValues(){
  Serial.print("left ");
  Serial.print(leftValue);
  Serial.print("\t");
  Serial.print("right ");
  Serial.print(rightValue);
  Serial.println();
}
