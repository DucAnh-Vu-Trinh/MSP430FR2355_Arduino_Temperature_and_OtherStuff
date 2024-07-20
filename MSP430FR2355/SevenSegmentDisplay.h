/*
 * SevenSegmentDisplay.h
 *
 *  Created on: Jul 7, 2024
 *      Author: trinh
 */

#ifndef SEVENSEGMENTDISPLAY_H_
#define SEVENSEGMENTDISPLAY_H_

#include "CustomGPIOConfig.h"

// Define the SevenSegmentDisplay struct
struct SevenSegmentDisplay {
    struct PinConfig disp_pins[7];  // Array for a-g pins of 7-Segment display of PinConfig structs
};

// Function prototype to initialize SevenSegmentDisplay
struct SevenSegmentDisplay* initSevenSegmentDisplay(
                             struct PinConfig a, struct PinConfig b,
                             struct PinConfig c, struct PinConfig d,
                             struct PinConfig e, struct PinConfig f,
                             struct PinConfig g);

void displayDigit(struct SevenSegmentDisplay* display, int digit);

#endif /* SEVENSEGMENTDISPLAY_H_ */
