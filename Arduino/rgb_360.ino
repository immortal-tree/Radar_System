#include <Servo.h>

const int trigPin1 = 6; 
const int echoPin1 = 5; 
const int trigPin2 = 7; 
const int echoPin2 = 8; 

const int buzzerPin = 4;
const int redPin = 9;
const int greenPin = 10;
const int bluePin = 11;

Servo myServo;
const int minThreshold = 20;
const int medThreshold = 40;
const int maxThreshold = 60;

void setup() {
  Serial.begin(9600);
  myServo.attach(3);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
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

  for (int angle = 0; angle <= 180; angle += 2) {
    myServo.write(angle);
    delay(50);
    int d1 = getDistance(trigPin1, echoPin1);
    int d2 = getDistance(trigPin2, echoPin2);

    int distance = min(d1, d2);

    LEDcolour(distance);
    Serial.print(angle);
    Serial.print(",");
    Serial.print(d1);
    Serial.print(",");
    Serial.println(d2);

    delay(200);
  }


  for (int angle = 180; angle >= 0; angle -= 2) {
    myServo.write(angle);
    delay(50);
    int d1 = getDistance(trigPin1, echoPin1);
    int d2 = getDistance(trigPin2, echoPin2);
    
    int distance = min(d1, d2);
    

    LEDcolour(distance);
    Serial.print(angle);
    Serial.print(",");
    Serial.print(d1);
    Serial.print(",");
    Serial.println(d2);

    delay(200);
  }
}

int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;
  
  //Serial.println(distance);

  return distance;
}

void LEDcolour(int distance) {
  if (distance < minThreshold) {
    digitalWrite(buzzerPin, HIGH);
    analogWrite(redPin, 255);
    analogWrite(greenPin, 0);
    analogWrite(bluePin, 0);
  } else if (distance >= maxThreshold && distance <= maxThreshold) {
    digitalWrite(buzzerPin, LOW);
    int red = map(distance, minThreshold, maxThreshold, 255, 0);
    int blue = map(distance, minThreshold, maxThreshold, 0, 255);
    analogWrite(redPin, red);
    analogWrite(greenPin,0);
    analogWrite(bluePin, blue);
   } else if (distance > medThreshold && distance < maxThreshold) {
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
