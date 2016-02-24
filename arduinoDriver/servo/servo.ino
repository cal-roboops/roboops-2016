#include <Servo.h>

// Constants
Servo myservo;
const int servoPin = 9;
int pos = 0;

void setup() {
  myservo.attach(9);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(20);
  }
  pos = 0;
}
