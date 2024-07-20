# MSP430FR2355-Arduino-Temperature-and-OtherStuff

<!-- GETTING STARTED -->
## Getting Started
### Prerequisites
:computer: Required Hardware
    <ul>
        <li>MSP430FR2355 Launchpad</li>
        <li>Arduino Board</li>
        <li>DHT11 Sensor</li>
        <li>2 7-Segment LEDs</li>
        <li>Breadboard</li>
        <li>Jump wires</li>
        <li>Resistors</li>
    </ul>

Optional Hardware
    <ul>
        <li>LEDs</li>
        <li>Blue-cap button</li>
        <li>IR Receiver</li>
        <li>Potentiometer</li>
        <li>RGB LEDs</li>
        <li></li>
    </ul>

:floppy_disk: Software
    <ul>
        <li>For the MSP430FR2355 code, it might be the easiest to just install **Code Composer IDE** from TI and add the MSP430FR2355 directories to the your workspace.</li>
        <li>For the Arduino code, it is obvious that you need to open the sketches (.ino file) in the **Arduino IDE**.</li>
    </ul>
### Installation
1. clone the repo
   ```sh
   git clone 'https://github.com/DucAnh-Vu-Trinh/MSP430FR2355_Arduino_Temperature_and_OtherStuff.git'
   ```
2. Add the MSP430FR2355 directory to Code Composer Workspace; or create a new project and add the source code there <details>
                    <summary>Project Settings</summary>
                    <ul>
                        <li>msp430fr2355 microcontroller</li>
                        <li>C project</li>
                    </ul>
</details>

3. Open the Arduino.ino in the Arduino directory with Arduino IDE

4. build the programe into MSP430FR2355 and Arduino

<!-- Usage -->
## Usage
### For MSP430FR2355
* :white_check_mark: MSP430FR2355 communicates with Arduino by **SPI Protocol**. MSP is the *MASTER* and Arduino is the *SLAVE*

* :white_check_mark: MSP430FR2355 use **TIMERB0** to request temperature data from the Arduino every *10 seconds*

* :white_check_mark: I write the **CustomGPIO** library so you can set up *Input/ Output* pin in MSP430FR2355 like you do in Arduino
    * Input pin with the Pull up/ Pull down resistor

* :white_check_mark: I write the **SevenSegment** library so you can quickly set up the *Seven Segment LEDs* with MSP430FR2355

* :white_check_mark: MSP430FR2355 samples voltages from the potentiometer by the **ADC Peripheral**

* :white_check_mark: MSP430FR2355 uses **UART** to transmit and receive data to and from the serial monitor

* :white_check_mark: MSP430FR2355 use **TIMERB1** to calculate *miliseconds* for stepper motor
### For Arduino
* :white_check_mark: Connect with DHT11 Temp/ Humidity sensors and send data to MSP430FR2355 by SPI
* :white_check_mark: Connect with a seven-segement led
* :white_check_mark: Connect with IR receiver
* :white_check_mark: Connect with RGB Led