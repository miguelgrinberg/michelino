/**
 * @file newping_distance_sensor.h
 * @brief distance sensor driver for distance sensors supported by the NewPing library.
 * @author Miguel Grinberg
 */

#include "distance_sensor.h"

namespace Michelino
{
    class DistanceSensor : public DistanceSensorDriver
    {
    public:
        DistanceSensor(int triggerPin, int echoPin, int maxDistance)
            : DistanceSensorDriver(maxDistance), 
              sensor(triggerPin, echoPin, maxDistance)
        {
        }
        
        virtual unsigned int getDistance()
        {
            int distance = sensor.ping_cm();
            if (distance <= 0)
                return maxDistance;
            return distance;
        }
    private:
        NewPing sensor;
    };
};
