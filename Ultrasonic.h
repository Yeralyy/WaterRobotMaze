#ifndef ULTRASONIC_H
#define ULTRASONIC_H

#include <Arduino.h>

class Ultrasonic {
    public:
        Ultrasonic(byte trigPin, byte echoPin) {
            _trigPin = trigPin;
            _echoPin = echoPin;
            pinMode(_trigPin, OUTPUT);
            pinMode(_echoPin, INPUT);
        }

        float getDistance() {
            if (_pulseTime <= 0) {
                _triggerUltrasonic();
                // return the distance
                return (_pulsTime * 0.0342) / 2
            }
            
            

        }

    private:
        byte _trigPin;
        byte _echoPin;
        // variable for timer
        uint32_t _tmr = 0; // in micros!!!
        // trigger interval
        uint32_t _triggerInterval = 50; // in micros!!!
        // pulse time
        uint32_t _pulseTime = 0;
    

        // function to trigger ultrasonic sensor
        void _triggerUltrasonic() {
            digitalWrite(_trigPin, LOW);

            if (millis() - _tmr >= 2) {
                // send waves:
                digitalWrite(_trigPin, HIGH);
                _tmr = micros();
            }

            // after sending pulse, measure the echo duration
            _pulseTime = pulseIn(_echoPin, HIGH);


            
        }
};

#endif