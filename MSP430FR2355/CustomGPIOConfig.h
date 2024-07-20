/*
 * CustomGPIOConfig.h
 *
 *  Created on: Jul 7, 2024
 *      Author: trinh
 */

#ifndef CUSTOMGPIOCONFIG_H_
#define CUSTOMGPIOCONFIG_H_

// Define constants for mode (INPUT and OUTPUT)
#define OUTPUT 1
#define INPUT 0

// Define constants for state (HIGH and LOW)
#define HIGH 1
#define LOW 0

// Structure to hold pin and port configuration
struct PinConfig {
    int port;
    int pin;
};

// Function prototype for pinMode
void pinMode(int port, int pin, int mode);

// Function prototype for digitalWrite
void digitalWrite(int port, int pin, int state);

#endif /* CUSTOMGPIOCONFIG_H_ */
