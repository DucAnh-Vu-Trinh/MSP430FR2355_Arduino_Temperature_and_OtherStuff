/*
 * CustomGPIOConfig.c
 *
 *  Created on: Jul 7, 2024
 *      Author: trinh
 */
#include "CustomGPIOConfig.h"
#include <stdint.h>
#include <msp430.h>

void pinMode(int port, int pin, int mode) {
    volatile uint8_t *dir_register;

    // Determine the direction register based on the port
    switch (port) {
        case 1:
            dir_register = &P1DIR;
            break;
        case 2:
            dir_register = &P2DIR;
            break;
        case 3:
            dir_register = &P3DIR;
            break;
        case 4:
            dir_register = &P4DIR;
            break;
        case 5:
            dir_register = &P5DIR;
            break;
        case 6:
            dir_register = &P6DIR;
            break;
        default:
            // Handle invalid port number
            return;
    }

    // Set or clear the pin direction based on the mode
    if (mode == OUTPUT) {
        *dir_register |= (1 << pin);    // Set pin as output
        digitalWrite(port, pin, LOW);
    } else if (mode == INPUT) {
        *dir_register &= ~(1 << pin);   // Set pin as input
    }
}

void digitalWrite(int port, int pin, int state) {
    volatile uint8_t *out_register;

    // Determine the output register based on the port
    switch (port) {
        case 1:
            out_register = &P1OUT;
            break;
        case 2:
            out_register = &P2OUT;
            break;
        case 3:
            out_register = &P3OUT;
            break;
        case 4:
            out_register = &P4OUT;
            break;
        case 5:
            out_register = &P5OUT;
            break;
        case 6:
            out_register = &P6OUT;
            break;
        default:
            // Handle invalid port number
            return;
    }

    // Set or clear the pin state based on the specified state (HIGH or LOW)
    if (state == HIGH) {
        *out_register |= (1 << pin);    // Set pin HIGH
    } else if (state == LOW) {
        *out_register &= ~(1 << pin);   // Set pin LOW
    }
}
