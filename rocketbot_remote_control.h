/**
 * @file rocketbot_remote_control.h
 * @brief remote control driver for the RocketBot Android remote control app.
 * @author Miguel Grinberg
 */

#include "remote_control.h"

namespace Michelino
{
    class RemoteControl : public RemoteControlDriver
    {
    public:
        /**
          * @brief Class constructor.
          * @param from the value in the 0-100 range that maps to 0 in remote control units.
              Any values below this one will be interpreted as 0.
          * @param to the value in the 0-100 range that maps to 255 in remote control units.
              Any values above this one will be interpreted as 255.
          */
        RemoteControl(int from, int to) : RemoteControlDriver(), 
            scaleFrom(from), scaleTo(to), 
            lastKey(command_t::keyNone), lastX(100), lastY(100), lastL(100), lastR(100) {}

        virtual bool getRemoteCommand(command_t& cmd)
        {
            cmd.stop();
            cmd.key = command_t::keyNone;
            
            if (BTSerial.available() < 2)
                return false; // no commands available
            unsigned char ch = BTSerial.read();
            unsigned char data = BTSerial.read();
            switch (ch) {
                case 'D': // keypad
                    switch (data) {
                    case 1:
                        cmd.goForward();
                        break;
                    case 2: // down
                        cmd.turnLeft();
                        break;
                    case 3: // left
                        cmd.turnRight();
                        break;
                    case 4: // right
                        cmd.goBack();
                        break;
                    case 5: // stop
                    default:
                        cmd.stop();
                        break;
                    }
                    break;
                case 'X':
                    lastX = data;
                    cmd.joystick(scale(lastX), -scale(lastY));
                    lastL = lastR = 100;
                    break;
                case 'Y':
                    lastY = data;
                    cmd.joystick(scale(lastX), -scale(lastY));
                    lastL = lastR = 100;
                    break;
                case 'L':
                    lastL = data;
                    cmd.leftAndRightSliders(scale(lastL), scale(lastR));
                    lastX = lastY = 100;
                    break;
                case 'R':
                    lastR = data;
                    cmd.leftAndRightSliders(scale(lastL), scale(lastR));
                    lastX = lastY = 100;
                    break;
                case 'A': // function keys
                    switch (data) {
                    case 1:
                        cmd.key = command_t::keyF1;
                        break;
                    case 2:
                        cmd.key = command_t::keyF2;
                        break;
                    case 3:
                        cmd.key = command_t::keyF3;
                        break;
                    case 4:
                        cmd.key = command_t::keyF4;
                        break;
                    }
                    break;
                default:
                    break;
            }
            if (cmd.key != command_t::keyNone && cmd.key == lastKey) {
                // repeated key, ignore it
                return false; 
            }
            lastKey = cmd.key;
            return true;
        }
    
    private:
        int scale(int value)
        {
            value -= 100;
            if (value >= 0) {
                if (value <= scaleFrom)
                    return 0;
                else if (value >= scaleTo)
                    return 255;
                return (value - scaleFrom) * 255 / (scaleTo - scaleFrom);
            }
            else {
                if (-value <= scaleFrom)
                    return 0;
                else if (-value >= scaleTo)
                    return -255;
                return -(-value - scaleFrom) * 255 / (scaleTo - scaleFrom);
            }
        }
        
    private:
        int scaleFrom;
        int scaleTo;
        command_t::key_t lastKey;
        int lastX;
        int lastY;
        int lastL;
        int lastR;
    };
};
