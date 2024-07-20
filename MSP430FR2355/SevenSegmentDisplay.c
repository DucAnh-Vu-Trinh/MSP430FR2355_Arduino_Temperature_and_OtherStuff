/*
 * SevenSegmentDisplay.cpp
 *
 *  Created on: Jul 7, 2024
 *      Author: trinh
 */
#include "SevenSegmentDisplay.h"
#include "CustomGPIOConfig.h"
#include <stdlib.h>

struct PinConfig disp_pins[7];  // Array for a-g pins of 7-Segment display of PinConfig structs
// Function to initialize SevenSegmentDisplay with PinConfig members
struct SevenSegmentDisplay* initSevenSegmentDisplay(
                             struct PinConfig a, struct PinConfig b,
                             struct PinConfig c, struct PinConfig d,
                             struct PinConfig e, struct PinConfig f,
                             struct PinConfig g) {
    struct SevenSegmentDisplay* display = (struct SevenSegmentDisplay*)malloc(sizeof(struct SevenSegmentDisplay));
    unsigned int i;
    if (display != NULL) {
        display->disp_pins[0] = a;
        display->disp_pins[1] = b;
        display->disp_pins[2] = c;
        display->disp_pins[3] = d;
        display->disp_pins[4] = e;
        display->disp_pins[5] = f;
        display->disp_pins[6] = g;

        // Initialize pin modes and ports for each disp_pin
        for (i = 0; i < 7; i++) {
            pinMode(display->disp_pins[i].port, display->disp_pins[i].pin, OUTPUT);
        }
    }

    return display;
    }

    void displayDigit(struct SevenSegmentDisplay* display, int digit) {
        switch(digit) {
            case 0:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, HIGH);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, HIGH);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, LOW);
                break;
            case 1:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, LOW);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, LOW);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, LOW);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, LOW);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, LOW);
                break;
            case 2:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, LOW);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, HIGH);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, LOW);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            case 3:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, LOW);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, LOW);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            case 4:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, LOW);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, LOW);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, LOW);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, HIGH);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            case 5:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, LOW);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, LOW);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, HIGH);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            case 6:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, LOW);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, HIGH);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, HIGH);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            case 7:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, LOW);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, LOW);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, LOW);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, LOW);
                break;
            case 8:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, HIGH);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, HIGH);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            case 9:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, HIGH);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, LOW);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, HIGH);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
            default:
                digitalWrite(display->disp_pins[0].port, display->disp_pins[0].pin, LOW);
                digitalWrite(display->disp_pins[1].port, display->disp_pins[1].pin, HIGH);
                digitalWrite(display->disp_pins[2].port, display->disp_pins[2].pin, HIGH);
                digitalWrite(display->disp_pins[3].port, display->disp_pins[3].pin, HIGH);
                digitalWrite(display->disp_pins[4].port, display->disp_pins[4].pin, HIGH);
                digitalWrite(display->disp_pins[5].port, display->disp_pins[5].pin, LOW);
                digitalWrite(display->disp_pins[6].port, display->disp_pins[6].pin, HIGH);
                break;
        }
   }
