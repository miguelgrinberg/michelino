/**
 * @file adafruit_motor_driver.h
 * @brief Motor device driver for the Adafruit motor shield.
 * @author Miguel Grinberg
 */

#include "motor_driver.h"

namespace Michelino
{
    class Motor : public MotorDriver
    {
    public:
        /*
         * @brief Class constructor.
         * @param number the DC motor number to control, from 1 to 4.
         */
        Motor(int number)
            : MotorDriver(), motor(number), currentSpeed(0)
        {
        }

        void setSpeed(int speed)
        {
            currentSpeed = speed;
            if (speed >= 0) {
                motor.setSpeed(speed);
                motor.run(FORWARD);
            }
            else {
                motor.setSpeed(-speed);
                motor.run(BACKWARD);
            }
        }
        
        int getSpeed() const
        {
            return currentSpeed;
        }
        
    private:
        AF_DCMotor motor;
        int currentSpeed;
    };
};
