#ifndef RDDisplay_h
#define RDDisplay_h

#include <Adafruit_NeoPixel.h>


// Each triad represents a single RGB colour
struct RGB {
   byte r;
   byte g;
   byte b;
};


class RDLEDDisplay {
  private:
      void displayLeftDigit(int index);
      void displayRightDigit(int index);
      void displayDigit(Adafruit_NeoPixel *strip, int index);
      void displayDigit(Adafruit_NeoPixel *strip, int index, RGB *color);
      void showPitStatus();
      void allOff();

      unsigned long previousMillis;
      char isDisplayingLeftDigit;
      char isDisplayingRightDigit;

      int isPoweringUpState;
      void powerUpLoop();
      int powerUpLED;

  public:
      void setup(int brightness);
      void loop(char leftDigit, char rightDigit);

};

#endif
