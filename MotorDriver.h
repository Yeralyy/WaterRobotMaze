#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <Arduino.h>

class Motor {
    public:
        // Constructor: Initialize motor pins and set them to OUTPUT
        Motor(byte in1, byte in2, byte in3, byte in4, byte ENA, byte ENB) {
            _in1 = in1;
            _in2 = in2;
            _in3 = in3;
            _in4 = in4;
            _ENA = ENA;
            _ENB = ENB;

            // Set pinMode for each motor control pin
            pinMode(_in1, OUTPUT);
            pinMode(_in2, OUTPUT);
            pinMode(_in3, OUTPUT);
            pinMode(_in4, OUTPUT);
            pinMode(_ENA, OUTPUT);
            pinMode(_ENB, OUTPUT);
        }

        // Set the speed for motor A (ENA)
        void setSpeedENA(uint8_t spd) {
            _ENASpeed = constrain(spd, 0, 255); // Ensure the speed is between 0 and 255
        }

        // Set the speed for motor B (ENB)
        void setSpeedENB(uint8_t spd) {
            _ENBSpeed = constrain(spd, 0, 255); // Ensure the speed is between 0 and 255
        }

        // Move the motors forward
        void forward() {
            // Set ENA speed (PWM control for motor A)
            analogWrite(_ENA, _ENASpeed);
            // Set direction for motor A
            digitalWrite(_in1, HIGH);
            digitalWrite(_in2, LOW);

            // Set ENB speed (PWM control for motor B)
            analogWrite(_ENB, _ENBSpeed);
            // Set direction for motor B
            digitalWrite(_in3, HIGH);
            digitalWrite(_in4, LOW);
        }

        // Move the motors backward
        void backward() {
            // Set ENA speed (PWM control for motor A)
            analogWrite(_ENA, _ENASpeed);
            // Set direction for motor A
            digitalWrite(_in1, LOW);
            digitalWrite(_in2, HIGH);

            // Set ENB speed (PWM control for motor B)
            analogWrite(_ENB, _ENBSpeed);
            // Set direction for motor B
            digitalWrite(_in3, LOW);
            digitalWrite(_in4, HIGH);
        }

        // Turn left
        void left() {
            // Set ENB speed (PWM control for motor B)
            analogWrite(_ENB, _ENBSpeed);
            // Set direction for motor B
            digitalWrite(_in3, LOW);
            digitalWrite(_in4, HIGH);

            // Set ENA speed (PWM control for motor A)
            analogWrite(_ENA, _ENASpeed);
            // Set direction for motor A
            digitalWrite(_in1, HIGH);
            digitalWrite(_in2, LOW);
        }

        // Turn right
        void right() {
            // Set ENB speed (PWM control for motor B)
            analogWrite(_ENB, _ENBSpeed);
            // Set direction for motor B
            digitalWrite(_in3, HIGH);
            digitalWrite(_in4, LOW);

            // Set ENA speed (PWM control for motor A)
            analogWrite(_ENA, _ENASpeed);
            // Set direction for motor A
            digitalWrite(_in1, LOW);
            digitalWrite(_in2, HIGH);
        }

        // Stop the motors
        void stop() {
            // Set all control pins to LOW to stop motors
            digitalWrite(_in1, LOW);
            digitalWrite(_in2, LOW);
            digitalWrite(_in3, LOW);
            digitalWrite(_in4, LOW);
        }

    private:
        byte _in1;
        byte _in2;
        byte _in3;
        byte _in4;

        byte _ENA;
        byte _ENB;
        
        uint8_t _ENASpeed = 0;
        uint8_t _ENBSpeed = 0;
};

#endif
