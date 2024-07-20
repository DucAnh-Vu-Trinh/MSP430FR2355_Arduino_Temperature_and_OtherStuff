/*
 * StepperMotor.h
 *
 *  Created on: Jul 8, 2024
 *      Author: trinh
 */

#ifndef STEPPERMOTOR_H_
#define STEPPERMOTOR_H_

#include <stdlib.h>
#include "CustomGPIOConfig.h"

extern volatile unsigned long milliseconds;

// Stepper motor structure
typedef struct {
    int direction;             // Direction of rotation
    unsigned long step_delay;  // Delay between steps, in ms, based on speed
    int number_of_steps;       // Total number of steps this motor can take
    int pin_count;             // How many pins are in use
    int step_number;           // Which step the motor is on

    // Motor pin numbers
    struct PinConfig motor_pin_1;
    struct PinConfig motor_pin_2;
    struct PinConfig motor_pin_3;
    struct PinConfig motor_pin_4;

    unsigned long last_step_time; // Timestamp in us of when the last step was taken
} Stepper;

// Function prototypes
void Stepper_init(Stepper *stepper, int number_of_steps, const struct PinConfig *motor_pin_1, const struct PinConfig *motor_pin_2, const struct PinConfig *motor_pin_3, const struct PinConfig *motor_pin_4);
void Stepper_setSpeed(Stepper *stepper, long whatSpeed);
void Stepper_step(Stepper *stepper, int number_of_steps);
int Stepper_version(void);
void Stepper_stepMotor(Stepper *stepper, int this_step);
unsigned long micros(void); // Placeholder function for getting the current time in microseconds
unsigned long millis(void);

int moduloBy4(int);


#endif /* STEPPERMOTOR_H_ */
