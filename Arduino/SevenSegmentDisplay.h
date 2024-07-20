#ifndef SEVENSEGMENTDISPLAY_H
#define SEVENSEGMENTDISPLAY_H

#include <Arduino.h>

class SevenSegmentDisplay {
public:
    SevenSegmentDisplay(int a, int b, int c, int d, int e, int f, int g);

    void setupPins();
    void displayDigit(int digit);

private:
    int SEG_A;
    int SEG_B;
    int SEG_C;
    int SEG_D;
    int SEG_E;
    int SEG_F;
    int SEG_G;
};

#endif // SEVENSEGMENTDISPLAY_H
