// Michelino
// Robot Vehicle firmware for the Arduino platform
//
// Copyright (c) 2013 by Miguel Grinberg
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is furnished
// to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
// AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

/**
 * @file michelino.ino
 * @brief Arduino robot vehicle firmware.
 * @author Miguel Grinberg
 */

#define LOGGING

// Device drivers
// Enable one driver in each category

// Motor controller:
#define ENABLE_ADAFRUIT_MOTOR_DRIVER

// Distance sensor
#define ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER

// Remote control:
//#define ENABLE_BLUESTICK_REMOTE_CONTROL_DRIVER
#define ENABLE_ROCKETBOT_REMOTE_CONTROL_DRIVER

// constants
#define TOO_CLOSE 10                    /**< distance to obstacle in centimeters */
#define MAX_DISTANCE (TOO_CLOSE * 20)   /**< maximum distance to track with sensor */
#define RANDOM_ANALOG_PIN 5             /**< unused analog pin to use as random seed */
#define BT_RX_PIN 16                    /**< RX pin for Bluetooth communcation */
#define BT_TX_PIN 17                    /**< TX pin for Bluetooth communcation */

#include <SoftwareSerial.h>
SoftwareSerial BTSerial(BT_RX_PIN, BT_TX_PIN);

#ifdef ENABLE_ADAFRUIT_MOTOR_DRIVER
#include <AFMotor.h>
#include "adafruit_motor_driver.h"
#define LEFT_MOTOR_INIT 1
#define RIGHT_MOTOR_INIT 3
#endif

#ifdef ENABLE_NEWPING_DISTANCE_SENSOR_DRIVER
#include <NewPing.h>
#include "newping_distance_sensor.h"
#define DISTANCE_SENSOR_INIT 14,15,MAX_DISTANCE
#endif

#ifdef ENABLE_BLUESTICK_REMOTE_CONTROL_DRIVER
#include "bluestick_remote_control.h"
#define REMOTE_CONTROL_INIT
#endif

#ifdef ENABLE_ROCKETBOT_REMOTE_CONTROL_DRIVER
#include "rocketbot_remote_control.h"
#define REMOTE_CONTROL_INIT 10,50
#endif

#include "logging.h"
#include "moving_average.h"

namespace Michelino
{
    class Robot
    {
    public:
        /*
         * @brief Class constructor.
         */
        Robot()
            : leftMotor(LEFT_MOTOR_INIT), rightMotor(RIGHT_MOTOR_INIT),
              distanceSensor(DISTANCE_SENSOR_INIT),
              distanceAverage(TOO_CLOSE * 10),
              remoteControl(REMOTE_CONTROL_INIT)
        {
            initialize();
        }
        
        /*
         * @brief Initialize the robot state.
         */
        void initialize()
        {
            randomSeed(analogRead(RANDOM_ANALOG_PIN));
            remote();
        }
        
        /*
         * @brief Update the state of the robot based on input from sensor and remote control.
         *  Must be called repeatedly while the robot is in operation.
         */
        void run()
        {
            unsigned long currentTime = millis();
            int distance = distanceAverage.add(distanceSensor.getDistance());
            RemoteControlDriver::command_t remoteCmd;
            bool haveRemoteCmd = remoteControl.getRemoteCommand(remoteCmd);
            log("state: %d, currentTime: %lu, distance: %u remote: (%d,l:%d,r:%d,k:%d)\n", 
                state, currentTime, distance, 
                haveRemoteCmd, remoteCmd.left, remoteCmd.right, remoteCmd.key);
            
            if (remoteControlled()) {
                if (haveRemoteCmd) {
                    switch (remoteCmd.key) {
                    case RemoteControlDriver::command_t::keyF1:
                        // start "roomba" mode
                        move();
                        break;
                    case RemoteControlDriver::command_t::keyNone:
                        // this is a directional command
                        leftMotor.setSpeed(remoteCmd.left);
                        rightMotor.setSpeed(remoteCmd.right);
                        break;
                    default:
                        break;
                    }
                }
            }
            else {
                // "roomba" mode
                if (haveRemoteCmd && remoteCmd.key == RemoteControlDriver::command_t::keyF1) {
                    // switch back to remote mode
                    remote();
                }
                else {
                    if (moving()) {
                        if (obstacleAhead(distance))
                            turn(currentTime);
                    }
                    else if (turning()) {
                        if (doneTurning(currentTime, distance))
                            move();
                    }
                }
            }
        }

    protected:
        void remote()
        {
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
            state = stateRemote;
        }
        
        void move()
        {
            leftMotor.setSpeed(255);
            rightMotor.setSpeed(255);
            state = stateMoving;
        }
        
        void stop()
        {
            leftMotor.setSpeed(0);
            rightMotor.setSpeed(0);
            state = stateStopped;
        }
        
        bool obstacleAhead(unsigned int distance)
        {
            return (distance <= TOO_CLOSE);
        }
        
        bool turn(unsigned long currentTime)
        {
            if (random(2) == 0) {
                leftMotor.setSpeed(-255);
                rightMotor.setSpeed(255);
            }
            else {
                leftMotor.setSpeed(255);
                rightMotor.setSpeed(-255);
            }
            state = stateTurning;
            endStateTime = currentTime + random(500, 1000);
        }
        
        bool doneTurning(unsigned long currentTime, unsigned int distance)
        {
            if (currentTime >= endStateTime)
                return (distance > TOO_CLOSE);
            return false;
        }
        
        bool moving() { return (state == stateMoving); }
        bool turning() { return (state == stateTurning); }
        bool stopped() { return (state == stateStopped); }
        bool remoteControlled() { return (state == stateRemote); }

    private:
        Motor leftMotor;
        Motor rightMotor;
        DistanceSensor distanceSensor;
        MovingAverage<unsigned int, 3> distanceAverage;
        RemoteControl remoteControl;
        enum state_t { stateStopped, stateMoving, stateTurning, stateRemote };
        state_t state;
        unsigned long endStateTime;
    };
};

Michelino::Robot robot;

void setup()
{
    Serial.begin(9600);
    BTSerial.begin(9600);
}

void loop()
{
    robot.run();
}
