#include <Arduino.h>
#include "Ultrasonic.h"
#include <OneButton.h>
#include "MotorDriver.h"
#include <SoftwareSerial.h>

bool BLUETOOTHFLAG = 1;

#define A0 14
#define A1 15
#define A2 16
#define A3 17

int ENA = 9;
int ENB = 3;

#define BT_RX 10
#define BT_TX 11

#define greenPin A4
#define bluePin A5
int redPin = 13;

int trig1Pin = 2;
int echo1Pin = 4;

int trig2Pin = 5;
int echo2Pin = 6;

int trig3Pin = 7;
int echo3Pin = 8;

int TRASHOLD = 50;


/*
GPIO'S LEVEL:
  LED pin's:
    green = A4  -> ENA
    blue = A5  -> ENB
    red = 13 
  Button = 12 
  Ultrasonics:
    trig1 = 2
    echo1 = 4
    trig2 = 5
    echo2 = 6
    trig3 = 7
    echo3 = 8


    
  motors:
    In1 = A0
    In2 = A1
    In3 = A2
    In4 = A3
    ENA = 9
    ENB = 3
  
  bluetooth:
    Rx = 10
    Tx = 11
    */






// flags

// mods of work
bool rightHandFlag = 0;
bool leftHandFlag = 0;
bool bluetoothFlag = 0;
bool greadyFlag = 0;

bool stopFlag = 0;



// Timers:

// button timer
uint32_t tmrButton = 0;

// flag check time
uint32_t tmrFlag = 0;

// timer for ultrasonic check:
uint32_t sonicTmr = 0; // 0 milliseconds

uint32_t tmr = 0;




// Periods:

// buton check period
int butCheckPeriod = 1000; // 10 milliseconds

// sonic period
int sonicPeriod = 50; // 50 milliseconds

// Flag period
int flagPeriod = 100; // 500 milliseconds

// create mtr object of Motor class
Motor mtr(A0, A1, A2, A3, ENA, ENB);

// create objects for ultrasonic sensor usage:
Ultrasonic sonic1(trig1Pin, echo1Pin); // left
Ultrasonic sonic2(trig2Pin, echo2Pin); // forward
Ultrasonic sonic3(trig3Pin, echo3Pin); // right

// Bluetooth serial
SoftwareSerial BTSerial(BT_RX, BT_TX);

OneButton btn;

void setup() {
  btn.setup(12, INPUT_PULLUP, true);
  //btn.setDebounceMs(50);
  // indeficators pins
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);

  btn.attachDoubleClick(doubleclick);
  btn.attachClick(oneclick);
  btn.attachMultiClick(tripleclick);

  mtr.setSpeedENA(170);
  mtr.setSpeedENB(170);


  // start Serial
  Serial.begin(9600);
  //start Bluetooth serial
  BTSerial.begin(9600);

} 


void loop() {

  if (millis() - tmrButton >= butCheckPeriod) {
    tmr = millis();
    btn.tick();
  }

  if (stopFlag) {
    mtr.stop();
    stopFlag = !stopFlag;
  }

  
  /* 
  Serial.print("Right Hand: ");
  Serial.print(rightHandFlag);
  Serial.print(" | ");
  Serial.print("Left Hand: ");
  Serial.print(leftHandFlag);
  Serial.print(" | ");
  Serial.print("Bluetooth mode: ");
  Serial.print(bluetoothFlag);
  Serial.println();
  */ 
  

  if (millis() - tmrFlag >= flagPeriod && rightHandFlag == 1) {
    tmrFlag = millis();
    
    // Right hand algorithm:
    if (millis() - sonicTmr >= sonicPeriod) {
      sonicTmr = millis();

      // get all directions distance
      float rightDistance = sonic3.getDistance();
      float forwardDistance = sonic2.getDistance();
      float leftDistance = sonic1.getDistance();
      
      
      Serial.print(rightDistance);
      Serial.print(" | ");
      Serial.print(forwardDistance);
      Serial.print(" | ");
      Serial.print(leftDistance);
      Serial.println();
      

      // check if right distance more than forward:
      if (rightDistance >= forwardDistance && rightDistance < TRASHOLD) {
          // turn right
          Serial.println("Turn Right");
          mtr.right();
      } else if (forwardDistance >= leftDistance && forwardDistance < TRASHOLD) { // else if forward distance more than left:
          // go forward
          Serial.println("Go Forward");
          mtr.forward(); 
      } else { // else 
          // turn left
          Serial.println("Turn Left");
          mtr.left();
      }
    }
  }

    // delay with timer
    if (millis() - tmrFlag >= flagPeriod && leftHandFlag == 1) {
    // set timer
    tmrFlag = millis();

    // Left hand algorithm
    if (millis() - sonicTmr >= sonicPeriod) {
      sonicTmr = millis();

      // get all direction's distance
      float rightDistance = sonic3.getDistance();
      float forwardDistance = sonic2.getDistance();
      float leftDistance = sonic1.getDistance();

      /*
      Serial.print(rightDistance);
      Serial.print(" | ");
      Serial.print(forwardDistance);
      Serial.print(" | ");
      Serial.print(leftDistance);
      Serial.println();
      */
      // if left direction have more space than forward:
      if (leftDistance >= forwardDistance && leftDistance < TRASHOLD) {
        // turn left
        //Serial.println("Turning left");
        mtr.left();
      } else if (forwardDistance >= rightDistance && forwardDistance < TRASHOLD) { 
        //Serial.println("Go forward");
        mtr.forward();
      } else {
        //Serial.println("Turning Right");
        mtr.right();
      }

      } 
    }

    if (millis() - tmrFlag >= flagPeriod && bluetoothFlag == 1) {
      tmrFlag = millis();

      // if there is something in Bluetooth Serial buffer:
      if (BTSerial.available() > 0) {

          // read command:
          char command = BTSerial.read();
          Serial.println(command);

          if (command == 'F') {
            //Serial.println("Moving forward | Bluetooth");
            mtr.forward();
          }
          else if (command == 'B') {
            //Serial.println("Moving backward | Bluetooth");
            mtr.backward();
          }
          else if (command == 'L') {
            //Serial.println("Moving left | Bluetooth");
            mtr.left();
          }
          else if (command == 'R') {
            //Serial.println("Moving right | Bluetooth");
            mtr.right();
          }
          else if (command == 'S') { 
            //Serial.println("Stopping | Bluetooth");
            mtr.stop();
          }
      }
    }
    /*
    if (millis() - tmrFlag >= flagPeriod && greadyFlag == 1) {
      tmrFlag = millis();

      float rightDistance = sonic3.getDistance();
      float forwardDistance = sonic2.getDistance();
      float leftDistance = sonic1.getDistance();

      if (forwardDistance > TRASHOLD && forwardDistance > leftDistance && forwardDistance > rightDistance) {
        mtr.forward();
      } else if (rightDistance > TRASHOLD && rightDistance > forwardDistance && rightDistance > leftDistance) {
        mtr.right();
      } else if (leftDistance > TRASHOLD && leftDistance > forwardDistance && leftDistance > rightDistance) {
        mtr.left();
      }

      
    }
    */

}



void doubleclick() {
  
  Serial.println("DOUBLE CLICK. LEFT HAND");
  // double click - left hand algorithm method:

  // setting flags
  leftHandFlag = 1;
  rightHandFlag = 0;
  bluetoothFlag = 0;

  // 3 colour indeficator (Blue)
  analogWrite(greenPin, 0);
  digitalWrite(redPin, LOW);
  analogWrite(bluePin, 255);
}

void oneclick() {

  Serial.println("ONE CLICK. RIGHT HAND");
  // one clik - right hand algorithm method

  // setting flags
  rightHandFlag = 1;
  leftHandFlag = 0;
  bluetoothFlag = 0;

  // 3 colour indeficator (green)
  analogWrite(greenPin, 255);
  digitalWrite(redPin, LOW);
  analogWrite(bluePin, 0);
}

void tripleclick(){

  Serial.println("TRIPPLE CLICK. BLUETOOTH");
  // triple click - bluetooth control

  // setting flags
  rightHandFlag = 0;
  leftHandFlag = 0;
  bluetoothFlag = 1;

  stopFlag = bluetoothFlag;

  // 3 colour indeficator (red)
  analogWrite(greenPin, 0);
  digitalWrite(redPin, HIGH);
  analogWrite(bluePin, 0);
}
