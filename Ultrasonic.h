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
            if (micros() - _tmr >= _triggerInterval) {
                _tmr = micros();
                _triggerUltrasonic();
            }
            
            if (_pulseTime > 0) {
                float distance = _calculateDistance(_pulseTime);
                _pulseTime = 0;
                return distance;
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

            if (micros() - _tmr >= 2) {
                // send waves:
                digitalWrite(_trigPin, HIGH);
                _tmr = micros();
            }

            // after sending pulse, measure the echo duration
            _pulseTime = pulseIn(_echoPin, HIGH);
            
        }

        float _calculateDistance(uint32_t pulseTime) {
            return (pulseTime * 0.0343) / 2;
        }

};

#endif