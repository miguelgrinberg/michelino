/**
 * @file distance_sensor.h
 * @brief distance sensor driver definition for the Michelino robot.
 * @author Miguel Grinberg
 */

namespace Michelino
{
    class DistanceSensorDriver
    {
    public:
        /**
          * @brief Class constructor.
          * @param distance The maximum distance in centimeters that needs to be tracked.
          */
        DistanceSensorDriver(unsigned int distance) : maxDistance(distance) {}
        
        /**
         * @brief Return the distance to the nearest obstacle in centimeters.
         * @return the distance to the closest object in centimeters 
         *   or maxDistance if no object was detected
         */
        virtual unsigned int getDistance() = 0;
        
    protected:
        unsigned int maxDistance;
    };
};
