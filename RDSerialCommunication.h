#ifndef RDSerialCommunication_h
#define RDSerialCommunication_h

#include "Arduino.h"

class RDSerialCommunication {
    private:
        void getDigitsForCommandD();
        void sendError();
    
        // serial port reading state
        int incoming_state; // 0=idle, 1=reading left, 2=reading right, 3=update
        char incoming_digit_left;
        char incoming_digit_right;
        char incoming_command;
    
        // Got these digits from serial port and are validated...:
        char currentLeftDigit;
        char currentRightDigit;

  public:
      void setup();
      void loop();
      char getCurrentLeftDigit();
      char getCurrentRightDigit();

};

#endif
