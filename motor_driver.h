/**
 * @file motor_driver.h
 * @brief Motor device driver definition for the Michelino robot.
 * @author Miguel Grinberg
 */

namespace Michelino
{
    class MotorDriver
    {
    public:
        /**
         * @brief Change the speed of the motor.
         * @param speed The new speed of the motor.
         *  Valid values are between -255 and 255. 
         *  Use positive values to run the motor forward, 
         *  negative values to run it backward and zero to stop the motor.
         */
        virtual void setSpeed(int speed) = 0;
        
        /**
         * @brief Return the current speed of the motor.
         * @return The current speed of the motor with range -255 to 255.
         */
        virtual int getSpeed() const = 0;            
    };
};
