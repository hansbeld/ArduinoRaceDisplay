#include "Arduino.h"
#include "RDLEDDisplay.h"

#define NUMBER_OF_LEDS_PER_DISPLAY  23
#define ARRAY_SENTINEL              9999

#define COLOR_R 255
#define COLOR_G 0
#define COLOR_B 0
#define COLOR_W 0

#define POWERUP_STATE_RIGHT 0
#define POWERUP_STATE_LEFT 1
#define POWERUP_STATE_DONE 2

#include <Adafruit_NeoPixel.h>
Adafruit_NeoPixel stripLeft = Adafruit_NeoPixel(NUMBER_OF_LEDS_PER_DISPLAY, 6, NEO_RGBW + NEO_KHZ800);
Adafruit_NeoPixel stripRight = Adafruit_NeoPixel(NUMBER_OF_LEDS_PER_DISPLAY, 5, NEO_RGBW + NEO_KHZ800);

// what leds do we need 'on' for these 10 numbers
const int zero[] = { 1,2,3,4,5,7,8,9,13,14,15,17,18,19,20,21,ARRAY_SENTINEL };
const int one[] = { 0,1,2,3,4,5,6,ARRAY_SENTINEL };
const int two[] = { 0,7,8,9,3,4,5,6,10,11,12,16,17,18,19,13,14,15,22,ARRAY_SENTINEL };
const int three[] = { 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,22,ARRAY_SENTINEL };
const int four[] = { 0,1,2,3,4,5,6,10,11,12,19,20,21,22,ARRAY_SENTINEL };
const int five[] = { 0,1,2,3,6,7,8,9,10,11,12,13,14,15,16,19,20,21,22,ARRAY_SENTINEL };
const int six[] =  { 0,1,2,3,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,ARRAY_SENTINEL };
const int seven[] = { 0,1,2,3,4,5,6,7,8,9,22,ARRAY_SENTINEL };
const int eight[] = { 1,2,3,4,5,7,8,9,10,11,12,13,14,15,17,18,19,20,21,ARRAY_SENTINEL };
const int nine[] = { 1,2,3,4,5,7,8,9,10,11,12,13,14,15,17,21,20,ARRAY_SENTINEL };

const int all_numbers[] = { zero,one,two,three,four,five,six,seven,eight,nine };

const int charP [] = { 4, 5, 6, 7, 8, 9, 10 , 11, 12, 16, 17, 18, 19, 20, 21, 22, ARRAY_SENTINEL };
const int charT [] = { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, ARRAY_SENTINEL };
const int colorRGB_PIT[] = { 34, 149, 206 }; // light blue-ish

void RDLEDDisplay::setup(int brightness) {
    stripLeft.begin();
    stripLeft.setBrightness(brightness);
    stripLeft.show();

    stripRight.begin();
    stripRight.setBrightness(brightness);
    stripRight.show();

    previousMillis = 0;
    powerUpLED = 0;
    isPoweringUpState = 0;

    isDisplayingLeftDigit = '\0';
    isDisplayingRightDigit = '\0';

    allOff();
}

void RDLEDDisplay::powerUpLoop() {
    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 90) 
    {
        previousMillis = currentMillis;

        if (powerUpLED >= NUMBER_OF_LEDS_PER_DISPLAY+1) {
            isPoweringUpState++;

            // final state
            if (isPoweringUpState == POWERUP_STATE_DONE) {
                allOff();
            }
            
            powerUpLED = 0;
        } else {
              if (isPoweringUpState == POWERUP_STATE_RIGHT) {
                  stripRight.setPixelColor(powerUpLED, powerUpLED*10, 255, 0, 0);
              }
              if (isPoweringUpState == POWERUP_STATE_LEFT) {
                  stripLeft.setPixelColor(powerUpLED, 0, 255, powerUpLED*10, 0);
              }                    

              stripLeft.show();
              stripRight.show();
          
              powerUpLED++;
        }
    }
}

void RDLEDDisplay::loop(char leftDigit, char rightDigit) {

    if (isPoweringUpState < 2) {
        powerUpLoop();

        return;
    }

    if (leftDigit <= 0 && rightDigit <= 0) {
        return;
    }

    if (leftDigit == '\0' || rightDigit == '\0') {
        return;
    }

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= 1000) {
        previousMillis = currentMillis;

        bool are_digits_changed = (isDisplayingLeftDigit != leftDigit || isDisplayingRightDigit != rightDigit);
        bool is_nn = (leftDigit == 'n' && rightDigit == 'n');

        if (are_digits_changed) {
            // leds keep their state. so, reset all:
            allOff();

            if (!is_nn) {
                // ascii to int
                int left = (int)leftDigit - 48;
                int right = (int)rightDigit - 48;


                if(left == 9 && right == 9) {
                  //pit status received
                  showPitStatus();
                } else {
                  displayLeftDigit(left);
                  displayRightDigit(right);
                }
                
                isDisplayingLeftDigit = leftDigit;
                isDisplayingRightDigit = rightDigit;
            } else {
                // 'dnn' = all off
                isDisplayingLeftDigit = '\0';
                isDisplayingRightDigit = '\0';
            }
        }
    }

    stripLeft.show();
    stripRight.show();
}

void RDLEDDisplay::allOff() {
    for (int n=0; n< NUMBER_OF_LEDS_PER_DISPLAY; n++) {
        stripLeft.setPixelColor(n, 0, 0, 0, 0);
        stripRight.setPixelColor(n, 0, 0, 0, 0);
    }

    stripLeft.show();
    stripRight.show();
}

void RDLEDDisplay::showPitStatus() {
  int *p = charP;
  while(*p != ARRAY_SENTINEL) {
    stripLeft.setPixelColor(*p, colorRGB_PIT[0], colorRGB_PIT[1], colorRGB_PIT[2], COLOR_W);
    p++;
  }

  int *t = charT;
  while(*t != ARRAY_SENTINEL) {
    stripRight.setPixelColor(*t, colorRGB_PIT[0], colorRGB_PIT[1], colorRGB_PIT[2], COLOR_W);
    t++;
  }
}


void RDLEDDisplay::displayRightDigit(int index) {
    int *curr = all_numbers[index];
    int *p = curr;

    while(*p != ARRAY_SENTINEL) {
        stripRight.setPixelColor(*p, COLOR_G, COLOR_R, COLOR_B, COLOR_W);
        p++;
    }
}

void RDLEDDisplay::displayLeftDigit(int index) {
    int *curr = all_numbers[index];
    int *p = curr;

    while(*p != ARRAY_SENTINEL) {
        stripLeft.setPixelColor(*p, COLOR_G, COLOR_R, COLOR_B, COLOR_W);
        p++;
    }
}
