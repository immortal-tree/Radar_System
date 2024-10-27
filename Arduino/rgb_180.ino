#include <Servo.h>

const int trigPin = 6;
const int echoPin = 5;

const int buzzerPin = 4;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

Servo myServo;
const int minThreshold = 20;
const int medThreshold = 50;
const int maxThreshold = 70;

void setup() {
  Serial.begin(9600);
  myServo.attach(3);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);
}

void loop() {
  for (int angle = 0; angle <= 180; angle += 2){
    myServo.write(angle);
    delay(50);
    int distance = getDistance();

    LEDcolour(distance);
    Serial.print(angle);
    Serial.print(",");
    
    delay(200);
  }

  for (int angle = 180; angle >= 0; angle -= 2){
    myServo.write(angle);
    delay(50);
    int distance = getDistance();

    LEDcolour(distance);
    Serial.print(angle);
    Serial.print(",");

    delay(200);
  }
}

int getDistance(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034/2;
  
  Serial.println(distance);

  return distance;
}

void LEDcolour(int distance){
  if (distance < minThreshold){
    digitalWrite(buzzerPin, HIGH);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else if (distance >= minThreshold && distance <= medThreshold){
    digitalWrite(buzzerPin, LOW);
    int red = map(distance, minThreshold, medThreshold, 255, 0);
    int blue = map(distance, minThreshold, medThreshold, 0, 255);
    analogWrite(redPin, red);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, blue);
  } else if (distance > medThreshold && distance < maxThreshold){
    digitalWrite(buzzerPin, LOW);
    int blue = map(distance, medThreshold, maxThreshold, 255, 0);
    int green = map(distance, medThreshold, maxThreshold, 0, 255);
    analogWrite(redPin, 0);
    analogWrite(greenPin, green);
    analogWrite(bluePin, blue);
  } else {
    digitalWrite(buzzerPin, LOW);
    analogWrite(redPin, 0);
    analogWrite(greenPin, 255);
    analogWrite(bluePin, 0);
  }
}
