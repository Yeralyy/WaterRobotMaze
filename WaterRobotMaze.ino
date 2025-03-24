#include <Arduino.h>
#include <Ultrasonic.h>
#include <OneButton.h>
#include <MotorDriver.h>
#include <SoftwareSerial.h>


#define A0 14
#define A1 15
#define A2 16
#define A3 17

#define ENA A4
#define ENB A5

#define BT_RX 10
#define BT_TX 11

int greenPin = 9;
int bluePin = 12;
int redPin = 13;

int trig1Pin = 2;
int echo1Pin = 4;

int trig2Pin = 5;
int echo2Pin = 6;

int trig3Pin = 7;
int echo3Pin = 8;


/*
GPIO'S LEVEL:
  LED pin's:
    green = 9 
    blue = 12
    red = 13
  Button = 3
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
    ENA = A4
    ENB = A5
  
  bluetooth:
    Rx = 10
    Tx = 11
    */


  



// flags

// mods of work
boolean rightHandFlag = false;
boolean leftHandFlag = false;
boolean bluetoothFlag = false;




// Timers:

// button timer
uint32_t tmrButton = 0;

// flag check time
uint32_t tmrFlag = 0;

// timer for ultrasonic check:
uint32_t sonicTmr = 0; // 0 milliseconds




// Periods:

// buton check period
int butCheckPeriod = 10; // 10 milliseconds

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
  btn.setup(3, INPUT_PULLUP, true);
  // indeficators pins
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(redPin, OUTPUT);

  btn.attachDoubleClick(doubleclick);
  btn.attachClick(oneclick);
  btn.attachMultiClick(tripleclick);

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

  if (millis() - tmrFlag >= flagPeriod && rightHandFlag) {
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
      if (rightDistance >= forwardDistance) {
          // turn right
          Serial.println("Turn Right");
          mtr.right();
      } else if (forwardDistance >= leftDistance) { // else if forward distance more than left:
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
    if (millis() - tmrFlag >= flagPeriod && leftHandFlag) {
    // set timer
    tmrFlag = millis();

    // Left hand algorithm
    if (millis() - sonicTmr >= sonicPeriod) {
      sonicTmr = millis();

      // get all direction's distance
      float rightDistance = sonic3.getDistance();
      float forwardDistance = sonic2.getDistance();
      float leftDistance = sonic1.getDistance();

      Serial.print(rightDistance);
      Serial.print(" | ");
      Serial.print(forwardDistance);
      Serial.print(" | ");
      Serial.print(leftDistance);
      Serial.println();
      
      // if left direction have more space than forward:
      if (leftDistance >= forwardDistance) {
        // turn left
        Serial.println("Turning left");
        mtr.left();
      } else if (forwardDistance >= rightDistance) { 
        Serial.println("Go forward");
        mtr.forward();
      } else {
        Serial.println("Turning Right")
        mtr.right();
      }

      } 
    }

    if (millis() - tmrFlag >= flagPeriod && bluetoothFlag) {
      tmrFlag = millis();

      // if there is something in Bluetooth Serial buffer:
      if (BTSerial.available() > 0) {
          // read command:
          char command = BTSerial.read();

          if (command == 'F') {
            Serial.println("Moving forward | Bluetooth");
            mtr.forward();
          }
          else if (command == 'B') {
            Serial.println("Moving backward | Bluetooth");
            mtr.backward();
          }
          else if (command == 'L') {
            Serial.println("Moving left | Bluetooth");
            mtr.left();
          }
          else if (command == 'R') {
            Serial.println("Moving right | Bluetooth");
            mtr.right();
          }
          else if (command == 'S') {
            Serial.println("Stopping | Bluetooth");
            mtr.stop();
          }
      }
    }

}



void doubleclick() {
  
  Serial.println("Double click. Left hand");
  // double click - left hand algorithm method:

  // setting flags
  leftHandFlag = true;
  rightHandFlag = false;
  bluetoothFlag = false;

  // 3 colour indeficator (Blue)
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, LOW);
  digitalWrite(bluePin, HIGH);
}

void oneclick() {

  Serial.println("One click. Right hand");
  // one clik - right hand algorithm method

  // setting flags
  rightHandFlag = true;
  leftHandFlag = false;
  bluetoothFlag = false;

  // 3 colour indeficator (green)
  digitalWrite(greenPin, HIGH);
  digitalWrite(redPin, LOW);
  digitalWrite(bluePin, LOW);
}

void tripleclick(){

  Serial.println("Tripple click. Bluetooth control");
  // triple click - bluetooth control

  // setting flags
  rightHandFlag = false;
  leftHandFlag = false;
  bluetoothFlag = true;

  // 3 colour indeficator (red)
  digitalWrite(greenPin, LOW);
  digitalWrite(redPin, HIGH);
  digitalWrite(bluePin, LOW);}
