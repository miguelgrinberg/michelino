/**
 * @file motorshield_motor_driver.h
 * @brief Motor device driver for the Arduino motor shield.
 * @author Nilesh Gulrajani edit by Will Ezell
 */
#include <Arduino.h>
#include "motor_driver.h"

namespace Michelino
{
    class Motor : public MotorDriver
    {
    public:
        /*
         * @brief Class constructor.
         * @param dirPin the direction pin (12 or 13)
         * @param speedPin the speed pin (3 or 11)
         * @param brakePin the brake pin (9 or 8)
         */
        Motor(int dirPin, int speedPin, int brakePin) 
            : MotorDriver(), motorDirPin(dirPin), motorSpeedPin(speedPin), motorBrakePin(brakePin), currentSpeed(0)
        {
        }

        void setSpeed(int speed)
        {
            currentSpeed = speed;
            if (speed > 0) {  //check the direction
                digitalWrite(motorDirPin, HIGH); //set direction pin to forward
	        digitalWrite(motorBrakePin, LOW); //disengage brake
	        analogWrite(motorSpeedPin, currentSpeed); //set speed pin to current speed
            }
            else if (speed < 0) { //check the direction
                digitalWrite(motorDirPin, LOW);// set direction pin to backward
                digitalWrite(motorBrakePin, LOW); //disengage brake
                analogWrite(motorSpeedPin, -currentSpeed); //set speed pin to current speed
            }
	    else { //check the direction
                digitalWrite(motorBrakePin, HIGH); //engage brake
                analogWrite(motorSpeedPin, 0); //set speed pin to 0
            }
        }
        
        int getSpeed() const
        {
            return currentSpeed;
        }
        
    private:
       	int motorSpeedPin;
	int motorDirPin;
	int motorBrakePin;
        int currentSpeed;
    };
};
