/* 
 *  2018, Dat is Hans, datishans.nl. 
 *  
 *  DON
 *  TCR
 *  ASH
 *  
 *  a BeLuKu racing division product
 *  
 */

#include "Arduino.h"
#include "RDSerialCommunication.h"
#include "RDLEDDisplay.h"

RDLEDDisplay ledDisplay;
RDSerialCommunication serial;

void setup() {
    ledDisplay.setup(255);
    serial.setup();
}

void loop() {
    serial.loop();
    
    char leftDigit = serial.getCurrentLeftDigit();
    char rightDigit = serial.getCurrentRightDigit();

    ledDisplay.loop(leftDigit, rightDigit);
}
