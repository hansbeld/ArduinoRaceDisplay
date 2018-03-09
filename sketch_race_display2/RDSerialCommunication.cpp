#include "Arduino.h"
#include "RDSerialCommunication.h"

#define COMMAND_DISPLAY 'd'

char RDSerialCommunication::getCurrentLeftDigit() {
    return currentLeftDigit;
}

char RDSerialCommunication::getCurrentRightDigit() {
    return currentRightDigit;
}

void RDSerialCommunication::setup() {
    incoming_state = 0;
    incoming_digit_left = '\0';
    incoming_digit_right = '\0';
    incoming_command = '\0';
    currentLeftDigit = '\0';
    currentRightDigit = '\0';

    Serial.begin(9600, SERIAL_8N1);

    while (!Serial) {
    ;
    }
}

void RDSerialCommunication::loop() {
    if (incoming_state == 0) {
        incoming_command = '\0';

        if (Serial.available() > 0) {
            char incoming_char = Serial.read();

            // allowed commands go here...
            if (incoming_char == COMMAND_DISPLAY) {
                incoming_digit_left = '\0';
                incoming_digit_right = '\0';
                incoming_command = incoming_char;

                incoming_state ++;
            }
        }
    } else {
        if (incoming_command == COMMAND_DISPLAY) {
            getDigitsForCommandD();
        }
    }
}

void RDSerialCommunication::getDigitsForCommandD() {
    switch (incoming_state) {
        case 1:
        {
            if (Serial.available() > 0) {
                incoming_digit_left = Serial.read();

                if ((incoming_digit_left  == 'n') || (incoming_digit_left >= '0' && incoming_digit_left <= '9')) {
                    incoming_state ++;
                } else {
                    sendError();
                    incoming_state = 0;
                }
            }
        }
        break;

        case 2:
        {
            if (Serial.available() > 0) {
                incoming_digit_right = Serial.read();

                if ((incoming_digit_right == 'n') || (incoming_digit_right >= '0' && incoming_digit_right <= '9')) {
                    incoming_state ++;
                } else {
                    sendError();
                    incoming_state = 0;
                }
            }
        }
        break;

        case 3:
        {
            if (Serial.availableForWrite()) {
                Serial.write(incoming_command);
                Serial.flush();
            }

            currentLeftDigit = incoming_digit_left;
            currentRightDigit = incoming_digit_right;
            
            incoming_state = 0;
            break;
        }
    }
}

void RDSerialCommunication::sendError() {
    if (Serial.availableForWrite()) {
        Serial.write('e');
        Serial.flush();
    }
}
