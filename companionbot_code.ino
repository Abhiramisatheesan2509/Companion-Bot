#include <Servo.h>

// Define pins for IR sensors
const int irSensorLeft = 2;
const int irSensorRight = 3;

// Define pins for ultrasonic sensor
const int trigPin = 9;
const int echoPin = 10;

// Define pins for L293 motor driver
const int motorPin1 = 4; // IN1
const int motorPin2 = 5; // IN2
const int motorPin3 = 6; // IN3
const int motorPin4 = 7; // IN4

// Define servo pin
const int servoPin = 11;

// Create Servo object
Servo myServo;

// Initialize variables for ultrasonic distance
long duration;
int distance;

void setup() {
  Serial.begin(9600);

  // Initialize IR sensor pins as input
  pinMode(irSensorLeft, INPUT);
  pinMode(irSensorRight, INPUT);

  // Initialize motor driver pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);
  pinMode(motorPin3, OUTPUT);
  pinMode(motorPin4, OUTPUT);

  // Initialize ultrasonic sensor pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  // Initialize Servo
  myServo.attach(servoPin);
  myServo.write(90);   // Set initial position
  delay(500);          // Wait for servo
}

void loop() {

  // Get the distance from the ultrasonic sensor
  distance = getDistance();

  // Print distance for debugging
  Serial.print("Ultrasonic Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // If obstacle is detected within 30 cm
  if (distance < 30) {
    moveForward();
    Serial.println("Obstacle detected! Moving Forward.");
  }
  else {
    stopMotors();
    Serial.println("No obstacle detected. Motors Stopped.");
  }

  // Sweep servo
  sweepServo();

  // IR sensor control for human detection
  if (digitalRead(irSensorLeft) == HIGH) {
    turnLeft();
    Serial.println("Human detected on the left. Turning Left.");
  }
  else if (digitalRead(irSensorRight) == HIGH) {
    turnRight();
    Serial.println("Human detected on the right. Turning Right.");
  }

  delay(100);
}

// Function to get distance from ultrasonic sensor
long getDistance() {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);

  long distanceCm = (duration / 2) / 29.1;

  return distanceCm;
}

// Function to move the robot forward
void moveForward() {

  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to stop the robot motors
void stopMotors() {

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, LOW);
}

// Function to sweep the servo from 30° to 150° and back
void sweepServo() {

  for (int angle = 30; angle <= 150; angle++) {
    myServo.write(angle);
    delay(50);
  }

  for (int angle = 150; angle >= 30; angle--) {
    myServo.write(angle);
    delay(50);
  }
}

// Function to turn robot left
void turnLeft() {

  digitalWrite(motorPin1, LOW);
  digitalWrite(motorPin2, HIGH);
  digitalWrite(motorPin3, HIGH);
  digitalWrite(motorPin4, LOW);
}

// Function to turn robot right
void turnRight() {

  digitalWrite(motorPin1, HIGH);
  digitalWrite(motorPin2, LOW);
  digitalWrite(motorPin3, LOW);
  digitalWrite(motorPin4, HIGH);
}
