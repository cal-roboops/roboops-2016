#include <BMSerial.h>
#include <RoboClaw.h>

//Roboclaw Constants
#define gmca 0x80
#define Kp 0x00010000
#define Ki 0x00008000
#define Kd 0x00004000
#define qpps 44000

// Constants
const int buttonPinF = 7;
const int buttonPinB = 8;

int buttonStateF = 0;
int buttonStateB = 0;

// sp is speed between 1 and 127
int spd = 50;

// For error checking
BMSerial terminal(0,1);

// RoboClaw (Rx, Tx)->(10,11)->(S2, S1)
RoboClaw rc(10, 11, 10000);

void setup() {   
  // Setup button and LED
  pinMode(buttonPinF, INPUT);
  pinMode(buttonPinB, INPUT);

  // Start all serials and set timers
  // (terminal, rc)
  terminal.begin(38400);
  rc.begin(38400);

  // Set roboclaw constants
  rc.SetM1VelocityPID(gmca,Kd,Kp,Ki,qpps);
  rc.SetM2VelocityPID(gmca,Kd,Kp,Ki,qpps);
}

void loop() {
  buttonStateF = digitalRead(buttonPinF);
  buttonStateB = digitalRead(buttonPinB);
  
  while (buttonStateF == 1) {
    rc.ForwardM1(gmca, spd);
    delay(400);

    buttonStateB = digitalRead(buttonPinB);
    if (buttonStateB) {
      buttonStateF = 0;
    }
    buttonStateF = digitalRead(buttonPinF) + 1;
  }
  while (buttonStateB == 1) {
    rc.BackwardM1(gmca, spd);
    delay(400);

    buttonStateF = digitalRead(buttonPinB);
    if (buttonStateF) {
      buttonStateB = 0;
    }
    buttonStateB = digitalRead(buttonPinB) + 1;
  }
  delay(400);
}

