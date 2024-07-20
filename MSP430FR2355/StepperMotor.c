/*
 * StepperMotor.c
 *
 *  Created on: Jul 8, 2024
 *      Author: trinh
 */

#include "StepperMotor.h"
#include "CustomGPIOConfig.h"

int current_step;

int moduloBy4(int num) {
    // Use bitwise AND operation with 0b11 (binary for 3)
    return num & 0b11;
}

// Constructor for four-pin version
void Stepper_init(Stepper *stepper, int number_of_steps, const struct PinConfig *motor_pin_1, const struct PinConfig *motor_pin_2, const struct PinConfig *motor_pin_3, const struct PinConfig *motor_pin_4) {
    stepper->step_number = 0;           // Which step the motor is on
    stepper->direction = 0;             // Motor direction
    stepper->last_step_time = 0;        // Timestamp in us of the last step taken
    stepper->number_of_steps = number_of_steps; // Total number of steps for this motor

    // Arduino pins for the motor control connection
    stepper->motor_pin_1 = *motor_pin_1;
    stepper->motor_pin_2 = *motor_pin_2;
    stepper->motor_pin_3 = *motor_pin_3;
    stepper->motor_pin_4 = *motor_pin_4;

    // Setup the pins on the microcontroller
    pinMode(stepper->motor_pin_1.port, stepper->motor_pin_1.pin, OUTPUT);
    pinMode(stepper->motor_pin_2.port, stepper->motor_pin_2.pin, OUTPUT);
    pinMode(stepper->motor_pin_3.port, stepper->motor_pin_1.pin, OUTPUT);
    pinMode(stepper->motor_pin_4.port, stepper->motor_pin_1.pin, OUTPUT);

    // pin_count is used by the stepMotor() method
    stepper->pin_count = 4;
}

// Set speed method
void Stepper_setSpeed(Stepper *stepper, long whatSpeed) {
    stepper->step_delay = 60L * 1000L * 1000L / stepper->number_of_steps / whatSpeed;
}

// Move method
void Stepper_step(Stepper *stepper, int steps_to_move) {
    int steps_left = abs(steps_to_move);  // How many steps to take

    // Determine direction based on whether steps_to_move is + or -
    if (steps_to_move > 0) { stepper->direction = 1; }
    if (steps_to_move < 0) { stepper->direction = 0; }

    // Decrement the number of steps, moving one step each time
    while (steps_left > 0) {
        unsigned long now = micros();
        // Move only if the appropriate delay has passed
        if (now - stepper->last_step_time >= stepper->step_delay) {
            // Get the timestamp of when you stepped
            stepper->last_step_time = now;
            // Increment or decrement the step number, depending on direction
            if (stepper->direction == 1) {
                stepper->step_number++;
                if (stepper->step_number == stepper->number_of_steps) {
                    stepper->step_number = 0;
                }
            } else {
                if (stepper->step_number == 0) {
                    stepper->step_number = stepper->number_of_steps;
                }
                stepper->step_number--;
            }
            // Decrement the steps left
            steps_left--;
            // Step the motor to step number 0, 1, 2, or 3
            current_step = moduloBy4(stepper->step_number);
            Stepper_stepMotor(stepper, moduloBy4(stepper->step_number));
        }
    }
    //TB1CTL &= ~MC_1;                // Stop Timer_B
}

// Step motor method
void Stepper_stepMotor(Stepper *stepper, int this_step) {

    switch (current_step) {
        case 0:  // 1010
            digitalWrite(stepper->motor_pin_1.port, stepper->motor_pin_1.pin, HIGH);
            digitalWrite(stepper->motor_pin_2.port, stepper->motor_pin_2.pin, LOW);
            digitalWrite(stepper->motor_pin_3.port, stepper->motor_pin_3.pin, HIGH);
            digitalWrite(stepper->motor_pin_4.port, stepper->motor_pin_4.pin, LOW);
            break;
        case 1:  // 0110
            digitalWrite(stepper->motor_pin_1.port, stepper->motor_pin_1.pin, LOW);
            digitalWrite(stepper->motor_pin_2.port, stepper->motor_pin_2.pin, HIGH);
            digitalWrite(stepper->motor_pin_3.port, stepper->motor_pin_3.pin, HIGH);
            digitalWrite(stepper->motor_pin_4.port, stepper->motor_pin_4.pin, LOW);
            break;
        case 2:  // 0101
            digitalWrite(stepper->motor_pin_1.port, stepper->motor_pin_1.pin, LOW);
            digitalWrite(stepper->motor_pin_2.port, stepper->motor_pin_2.pin, HIGH);
            digitalWrite(stepper->motor_pin_3.port, stepper->motor_pin_3.pin, LOW);
            digitalWrite(stepper->motor_pin_4.port, stepper->motor_pin_4.pin, HIGH);
            break;
        case 3:  // 1001
            digitalWrite(stepper->motor_pin_1.port, stepper->motor_pin_1.pin, HIGH);
            digitalWrite(stepper->motor_pin_2.port, stepper->motor_pin_2.pin, LOW);
            digitalWrite(stepper->motor_pin_3.port, stepper->motor_pin_3.pin, LOW);
            digitalWrite(stepper->motor_pin_4.port, stepper->motor_pin_4.pin, HIGH);
            break;
    }
}

// Version method
int Stepper_version(void) {
    return 1;
}

// Placeholder function to get the current time in microseconds
unsigned long micros(void) {
    return (unsigned long)(millis() * 1000000); // Convert milliseconds to microseconds
}

unsigned long millis(void) {
    unsigned long millis_copy;

    __disable_interrupt();           // Disable interrupts to prevent race conditions.
    millis_copy = milliseconds;
    __enable_interrupt();            // Re-enable interrupts.

    return millis_copy;
}
