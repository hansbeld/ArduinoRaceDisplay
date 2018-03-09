#ifndef RDDisplay_h
#define RDDisplay_h

class RDLEDDisplay {
  private:
      void displayLeftDigit(int index);
      void displayRightDigit(int index);
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
