/**
 * @file remote_control.h
 * @brief remote control driver definition for the Michelino robot.
 * @author Miguel Grinberg
 */

namespace Michelino
{
    class RemoteControlDriver
    {
    public:
        /**
          * @brief abstract representation of a remote command.
          */
        struct command_t {
            enum key_t { keyNone, keyF1, keyF2, keyF3, keyF4 };
            int left;   /**< left side speed, between -255 and 255. */
            int right;  /**< right side speed, between -255 and 255. */
            key_t key;  /**< function key. */
            
            command_t() : left(0), right(0), key(keyNone) {}
            void goForward()
            {
                left = right = 255;
            }
            void goBack()
            {
                left = right = -255;
            }
            void turnLeft()
            {
                left = -255;
                right = 255;
            }
            void turnRight()
            {
                left = 255;
                right = -255;
            }
            void stop()
            {
                left = right = 0;
            }
            void leftAndRightSliders(int l, int r)
            {
                left = l;
                right = r;
            }
            void forwardBackAndLeftRightSliders(int fb, int lr)
            {
                left = fb - lr;
                right = fb + lr;
                if (left < -255)
                    left = -255;
                else if (left > 255)
                    left = 255;
                if (right < -255)
                    right = -255;
                else if (right > 255)
                    right = 255;
            }
            void joystick(int x, int y)
            {
                forwardBackAndLeftRightSliders(y, x);
            }
        };
        
        /**
          * @brief Class constructor.
          */
        RemoteControlDriver() {}

        /**
         * @brief Return the next remote command, if available.
         * @param cmd a reference to a command_t struct where the command
         *   information will be stored.
         * @return true if a remote command is available, false if not.
         */
        virtual bool getRemoteCommand(command_t& cmd) = 0;
    };
};
