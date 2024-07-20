#include <msp430.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "CustomGPIOConfig.h"
#include "SevenSegmentDisplay.h"
#include "StepperMotor.h"

volatile unsigned int count = 0;
volatile unsigned long milliseconds = 0;

volatile unsigned int seconds = 0;
volatile unsigned int minutes = 0;
volatile unsigned int hours = 0;
volatile unsigned int days = 0;
char message[10]; // Array to hold the string representation of seconds

volatile uint8_t Rx_Data;
volatile uint8_t trueTemp;
volatile bool MasterReceived = false; // Flag to indicate Master Received data from Slave
volatile bool ADCConversion = true;
volatile bool UpdateMessage = false;
volatile bool RotateStepper = false;

struct SevenSegmentDisplay* display1;
Stepper stepper1;

unsigned int position;
unsigned int ADC_Value;

char packet[] = {0xFF};

void delay_ms(unsigned int ms);

void TimerB0_init(void);
void TimerB1_init(void);
void configureGPIO(void);
void UART_init(void);
void SPI_init(void);
void SevenSegment_init(void);
void StepperMotor_init(void);
void ADC_init(void);
void formatTime(unsigned int hours, unsigned int minutes, unsigned int seconds, char *message);

uint8_t reverseBits(uint8_t);

/**
 * main.c
 */
void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop the Watchdog timer

    configureGPIO();
    TimerB0_init();
    TimerB1_init();
    UART_init();
    SPI_init();
    ADC_init();
    StepperMotor_init();

    __enable_interrupt();       // Global enable for maskable IRQs
    while (1) {
        if (MasterReceived){
            trueTemp = reverseBits(Rx_Data);
            displayDigit(display1, trueTemp);
            MasterReceived = false;
        }

        if (UpdateMessage){
            formatTime(hours, minutes, seconds, message);
            position = 0;
            UCA1IE |= UCTXCPTIE;        // turns on Tx complete IRQ
            UCA1IFG &= ~UCTXCPTIFG;      // clear Tx complete flag
            UCA1TXBUF = message[position];      // put first char from message into Tx buff
            UpdateMessage = false;
        }

        if (RotateStepper){
            Stepper_step(&stepper1, 2000);
            RotateStepper = false;
        }

        ADCCTL0 |= ADCENC | ADCSC;      // Enable and Start Conv

        //__bis_SR_register(LPM0_bits | GIE); // Enter LPM0 with interrupts enabled (SMCLK active)
        __no_operation();                   // For debugger
    }
}

void configureGPIO(void)
{
    //-- Setup Port 1.2
    P1DIR |= BIT2;              // Set P1.2 to output direction (LED)
    P1OUT &= ~BIT2;             // Turn off LED

    //-- Setup Port 4.1
    P4DIR &= ~BIT1;            // Make P4.1 in input (SW1)
    P4REN |= BIT1;
    P4OUT |= BIT1;             // Pull up resistor
    P4IES |= BIT1;             // Make IRQ H-to-L

    //-- Setup Port 2.3
    P2DIR &= ~BIT3;            // Make P4.1 in input (SW1)
    P2REN |= BIT3;
    P2OUT |= BIT3;             // Pull up resistor
    P2IES |= BIT3;             // Make IRQ H-to-L

    //-- Setup Port 1.0
    P1DIR |= BIT0;             // LED1
    P1OUT &= ~BIT0;            // Clear P1.0

    //-- Setup Port 6.6
    P6DIR |= BIT6;             // LED2
    P6OUT &= ~BIT6;            // Clear P6.6

    //-- Setup Port 4.3
    P4SEL1 &= ~BIT3;           // Set P4.3 to use UART A1 Tx function
    P4SEL0 |= BIT3;

    //Setup Port 1.4
    P1SEL1 &= ~BIT4;           // Set P1.4 to use UCA0STE
    P1SEL0 |= BIT4;

    //-- Setup Port 1.5
    P1SEL1 &= ~BIT5;           // Set P1.5 to use SCLK
    P1SEL0 |= BIT5;

    //-- Setup Port 1.6
    P1SEL1 &= ~BIT6;           // Set P1.6 to use SOMI
    P1SEL0 |= BIT6;

    //-- Setup Port 1.7
    P1SEL1 &= ~BIT7;           // Set P1.7 to use SIMO
    P1SEL0 |= BIT7;

    //-- Setup Port 4.2
    P4SEL1 &= ~BIT2;           // Set P4.2 to use UART A1 Rx function
    P4SEL0 |= BIT2;

    //-- Setup 7 Pins for Seven Segment LED
    SevenSegment_init();       // Set pin connect to 7 segment to output

    //--Setup 4 Pins for Stepper Motor
    StepperMotor_init();

    //-- Setup Port 3.2
    P3DIR &= ~BIT2;            // Make P3.2 input
    P3REN &= ~BIT2;
    P3IES |= BIT2;             // Make IRQ H-to-L

    //-- Setup Port 5.3 (A11)
    P5SEL1 |= BIT3;            // Setup Analog A11
    P5SEL0 |= BIT3;

    PM5CTL0 &= ~LOCKLPM5;      // Enable digital I/O

    //-- Enable Port 4 IRQs
    P4IE |= BIT1;              // Enable SW1 IRQ
    P4IFG &= ~BIT1;            // Clear flag

    //-- Enable Port 3 IRQs
    P3IE |= BIT2;              // Enable SW1 IRQ
    P3IFG &= ~BIT2;

    //-- Enable Port 2 IRQs
    P2IE |= BIT3;              // Enable SW1 IRQ
    P2IFG &= ~BIT3;            // Clear flag

}

void delay_ms(unsigned int ms) {
    unsigned int i;
    while (ms--) {
        for (i = 0; i < 1000; i++) {
            __no_operation(); // Replace with NOP instruction
        }
    }
}

void SevenSegment_init(void) {
    const struct PinConfig a = {3, 6};
    const struct PinConfig b = {6, 0};
    const struct PinConfig c = {6, 1};
    const struct PinConfig d = {5, 2};
    const struct PinConfig e = {4, 5};
    const struct PinConfig f = {3, 4};
    const struct PinConfig g = {1, 3};
    display1 = initSevenSegmentDisplay(a, b, c, d, e, f, g);
}

void StepperMotor_init(void) {
    const long speed = 5;
    const struct PinConfig pin1 = {3, 0};
    const struct PinConfig pin2 = {2, 5};
    const struct PinConfig pin3 = {3, 2};
    const struct PinConfig pin4 = {3, 3};
    Stepper_init(&stepper1, 2000, &pin1, &pin2, &pin3, &pin4);   // 2000 steps per rotation
    Stepper_setSpeed(&stepper1, speed);   // Set the speed to 5 rpm
}

void TimerB0_init(void)
{
    //-- Setup timer
    TB0CTL = TBSSEL__ACLK | MC__UP | TBCLR; // ACLK, up mode, clear TBR
    TB0CCR0 = 32768;      // Set CCR0 value for 1 second interval (assuming ACLK @ 32.768kHZ)
    //-- Setup Timer Compare IRQ

    //TB0CTL |= TBIE;             // local enable for *overflow* in TBO
    //TB0CTL &= ~TBIFG;           // Clear flag (not using this Overflow flag)

    TB0CCTL0 |= CCIE;       //local enable for *CCR0*
    TB0CCTL0 &= ~CCIFG;           // Clear flag
}

void TimerB1_init(void){
    //-- Setup timer
    TB1CTL = TBSSEL__SMCLK | TBCLR | MC_1; // SMCLK, up mode, clear TBR
    TB1CCR0 = 1000;      // Set CCR0 value for 1 milisecond interval (assuming SMCLK @ 1MHZ)
    //-- Setup Timer Compare IRQ

    //TB0CTL |= TBIE;             // local enable for *overflow* in TBO
    //TB0CTL &= ~TBIFG;           // Clear flag (not using this Overflow flag)

    TB1CCTL0 |= CCIE;       //local enable for *CCR1*
    TB1CCTL0 &= ~CCIFG;           // Clear flag
}

void UART_init(void){
    //-- Setup UCA1
    UCA1CTLW0 |= UCSWRST | UCSSEL__SMCLK;       //puts UART A1 into SW reset; BRCLK = SMCLK --> 115200 baud
    UCA1BRW = 8;        // prescalar = 0
    UCA1MCTLW = 0xD600;     // sets low freq mode + 2nd mod stage

    UCA1CTLW0 &= ~UCSWRST;      // take UART A1 out of SW reset

    //-- Setup UCA0 RXIFG
    UCA1IE |= UCRXIE;       // local enable for A1 RXIFG
}

void SPI_init(void){
    UCA0CTLW0 |= UCSWRST;       // put A0 to SW Reset
    UCA0CTLW0 |= UCSSEL__SMCLK; // choose SMLCK
    UCA0BRW = 10;       // set prescalar to 10 to get SCLK = 1M/10 = 100kHZ

    UCA0CTLW0 |= UCSYNC;    // put A0 to SPI mode
    UCA0CTLW0 |= UCMST;     // put into SPI Master

    UCA0CTLW0 |= UCMODE1;
    UCA0CTLW0 &= ~UCMODE0;  // 4-pin Mode, active LOW STE

    UCA0CTLW0 |= UCSTEM;    // output enable for SLAVE

    UCA0CTLW0 &= ~UCSWRST;      // take SPI A0 out of SW Reset

    UCA0IE |= UCTXIE;       // enable A0 Tx IRQ
    UCA0IE |= UCRXIE;       // enable A0 Rx IRQ
    UCA0IFG &= ~UCTXIFG;    // clear flag
    UCA0IFG &= ~UCRXIFG;    // clear flag
}

void ADC_init(void){
    ADCCTL0 &= ~ADCSHT;     // set conv clock cycles = 16 (10)
    ADCCTL0 |= ADCSHT_2;
    ADCCTL0 |= ADCON;       // turn on ADC

    ADCCTL1 |= ADCSSEL_2;   // choose SMCLK
    ADCCTL1 |= ADCSHP;      // sample signal source =sampling timer;

    ADCCTL2 &= ~ADCRES;     // clear resolution
    ADCCTL2 |= ADCRES_2;    // 12-bit resolution

    ADCMCTL0 |= ADCINCH_11; // ADC input = A11 (P5.3)

    //-- Setup IRQ
    ADCIE |= ADCIE0;        // Conversion complete IRQ
}

void formatTime(unsigned int hours, unsigned int minutes, unsigned int seconds, char *message) {
    // Lookup tables for tens and units digits
    const char tens_digits[] = "0000000001111111111222222222233333333334444444444555555555666666666777777777788888888889999999999";
    const char units_digits[] = "0123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789";
    message[0] = tens_digits[hours];
    message[1] = units_digits[hours];
    message[2] = ':';
    message[3] = tens_digits[minutes];
    message[4] = units_digits[minutes];
    message[5] = ':';
    message[6] = tens_digits[seconds];
    message[7] = units_digits[seconds];
    message[8] = ' ';
    message[9] = '\0'; // Null terminator;
}

uint8_t reverseBits(uint8_t n) {
    int i;
    uint8_t result = 0;
    for (i = 7; i >= 0; i--) {
        result <<= 1;          // Shift result left by 1
        result |= (n & 1);     // Copy the least significant bit of n to result
        n >>= 1;               // Shift n right by 1
    }
    return result;
}

//-- Timer B0 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__) // Check if using TI or IAR compiler
#pragma vector = TIMER0_B0_VECTOR   // Interrupt vector declaration for Timer_B0
__interrupt void ISR_Timer_B0(void)
#elif defined(__GNUC__) // Check if using GCC compiler
void __attribute__ ((interrupt(TIMER0_B0_VECTOR))) Timer_B (void)   // Interrupt service routine for Timer_B0 with GCC attribute syntax
#else
#error Compiler not supported!  // Error message if using an unsupported compiler
#endif
{
    seconds++;
    if (seconds == 60)
    {
        seconds = 0;
        minutes++;
        if (minutes == 60)
        {
            minutes = 0;
            hours++;
            if (hours == 24)
            {
                hours = 0;
                days++;
            }
        }
    }

//    P1OUT ^= BIT2; // Toggle LED every second
    if (seconds % 10 == 0){ // send data over SPI every 10 seconds
        position = 0;
        UCA0TXBUF = packet[position];       // Tx packet[position] out over SPI
    }

    TB0CCTL0 &= ~CCIFG;     // clear flag
}

#pragma vector = TIMER1_B0_VECTOR   // Interrupt vector declaration for Timer_B1
__interrupt void ISR_Timer_B1(void){
    count++;
    milliseconds++;
    if (count == 1000){
        P1OUT ^= BIT2; // Toggle LED every second
        count = 0;
    }
    TB1CCTL0 &= ~CCIFG;     // clear flag
}

//-- SW1 ISR
#pragma vector = PORT4_VECTOR
__interrupt void ISR_Port4_SW1(void){
    UpdateMessage = true;
    P4IFG &= ~BIT1;     //Clear flag for SW1
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 after ISR
}

//-- SW2 ISR
#pragma vector = PORT2_VECTOR
__interrupt void ISR_Port2_SW2(void){
//    position = 0;
//    UCA0TXBUF = packet[position];       // Tx packet[position] out over SPI
//
//    P2IFG &= ~BIT3;     // Clear flag for SW2
    RotateStepper = true;
    P2IFG &= ~BIT3;     // Clear flag for SW2
}

//-- A1 UART Tx ISR
#pragma vector = EUSCI_A1_VECTOR
__interrupt void ISR_EUSCI_A1(void){
    if (UCA1IE & UCTXCPTIE){
        if(position == sizeof(message)){
                UCA1IE &= ~UCTXCPTIE;        // turn off Tx complete IRQ
        }
        else{
            position++;
            UCA1TXBUF = message[position];       // put next char into Tx buff
        }
        UCA1IFG &= ~UCTXCPTIFG;       // clear Tx complete flag
    }
    else{
        if (UCA1RXBUF == 't'){
            P1OUT ^= BIT0;     // toggle LED1
        }
    }
}

#pragma vector = EUSCI_A0_VECTOR    // Data is in A0 SPI buffer or Data completes sent out
__interrupt void ISR_EUSCI_A0(void){
    if (UCA0IFG & UCTXIFG){
        position++;
        if(position < sizeof(packet)){
            UCA0TXBUF = packet[position];
        }
        else{
            UCA0IFG &= ~UCTXIFG;
        }
    }
    else{
        Rx_Data = UCA0RXBUF;    // Read Rx buffer
        P6OUT ^= BIT6;     // toggle LED2
        MasterReceived = true;
        __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 after ISR
    }
}

//-- PORT3 ISR
#pragma vector = PORT3_VECTOR
__interrupt void ISR_Port3(void){
    //TB1CTL |= MC_1;                 // Start Timer_B in up mode
    Stepper_step(&stepper1, 1000);
    P3IFG &= ~BIT2;     //Clear flag
}

//-- ADC ISR
#pragma vector = ADC_VECTOR
__interrupt void ISR_ADC(void){
    ADC_Value = ADCMEM0;
    __bic_SR_register_on_exit(LPM0_bits); // Exit LPM0 after ISR
}
