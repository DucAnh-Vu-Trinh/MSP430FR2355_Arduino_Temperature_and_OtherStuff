/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-dht11
 */
#include <Arduino.h>
#include "DHT.h"
#include "SevenSegmentDisplay.h"
#include "AccelStepper.h"
#include "IRremote.hpp"
#include "RemoteController.h"
#include <SPI.h>
#define DHT11_PIN 2
#define PIN_RED 5
#define PIN_GREEN 9
#define PIN_BLUE 6
#define IR_RECEIVE_PIN A3

volatile byte Slavereceived;
volatile unsigned int elapsedSeconds = 0;
uint8_t slaveSend;

volatile bool mspIsLive = false;

char sendMessage[100];
char humiStr[8];
char tempCStr[8];
char tempFStr[8];

unsigned long previousMillisDHT = 0;  // Stores the last time the seconds were updated
unsigned long previousMillisSPI = 0;  // Stores the last time the seconds were updated
unsigned long currentMillis = 0;

int x = 0;

DHT dht11(DHT11_PIN, DHT11); // Create an instance of teh DHT class
SevenSegmentDisplay sevenSegDisp(A5, A4, 7, A1, A2, 4, 3); // Create an instance of the SevenSegmentDisplay class
RemoteController remoteController(PIN_RED, PIN_GREEN, PIN_BLUE);  // Create an instance of the remoteCOntroller class

void setup() {
  
  //-- Setup Temp and Humid sensor
  dht11.begin(); // initialize the sensor

  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);

  //-- Setup GPIO
  pinMode(PIN_RED, OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE, OUTPUT);
  pinMode(MISO, OUTPUT);
  sevenSegDisp.setupPins(); // initialize the seven segment

  //-- Initalize communication protocol
  Serial.begin(115200, SERIAL_8N1); // initialize the UART
  delay(2000); // Give some time for Serial to initialize
  SPI.beginTransaction(SPISettings(100000, MSBFIRST, SPI_MODE1)); // initialize SPI config
  SPCR |= _BV(SPE); // Set Arduino to SLAVE

  SPI.attachInterrupt(); // Enable SPI Interupt
}

void loop() {
  currentMillis = millis();   // Get current time
  if (currentMillis - previousMillisDHT >= 5000) {// wait a few seconds between measurements.
    // Update previousMillis to currentMillis
    previousMillisDHT = currentMillis;
    // read humidity
    float humi  = dht11.readHumidity();
    // read temperature as Celsius
    float tempC = dht11.readTemperature();
    // read temperature as Fahrenheit
    float tempF = dht11.readTemperature(true);

    int tempFDigit = static_cast<int>(tempF);
    int tempFFirstDigit = tempFDigit % 10;
    int tempFSecondDigit = (tempFDigit - tempFFirstDigit) / 10;

    slaveSend = (uint8_t) tempFSecondDigit;

    // check if any reads failed
    if (isnan(humi) || isnan(tempC) || isnan(tempF)) {
      Serial.println("Failed to read from DHT11 sensor!");
    } else {
      dtostrf(humi, 6, 2, humiStr);
      dtostrf(tempC, 6, 2, tempCStr);
      dtostrf(tempF, 6, 2, tempFStr);
      sprintf(sendMessage, "DHT11# Humidity: %s%% | Temperature: %s°C ~ %s°F", humiStr, tempCStr, tempFStr);
      // Serial.print("DHT11# Humidity: ");
      // Serial.print(humi);
      // Serial.print("%");

      // Serial.print("  |  "); 

      // Serial.print("Temperature: ");
      // Serial.print(tempC);
      // Serial.print("°C ~ ");
      // Serial.print(tempF);
      // Serial.println("°F");
      Serial.println(sendMessage);
      // Serial.println(slaveSend);
    
      sevenSegDisp.displayDigit(tempFFirstDigit);

      if (!mspIsLive){
        Serial.println("MSP is not live");
        if (!remoteController.status){
          if (tempF > 70) {
            analogWrite(PIN_GREEN, 10);
          }
          else{
            analogWrite(PIN_GREEN, 0);
          }
        }
      }
      else{
        if (!remoteController.status){
          analogWrite(PIN_GREEN, 0);
        }
      }
    }
  }
  if (currentMillis - previousMillisSPI >= 1000){
    previousMillisSPI = currentMillis;
    elapsedSeconds++;
  }
  if (elapsedSeconds >= 31){
    mspIsLive = false;
  }

  if (IrReceiver.decode()) {
    IrReceiver.printIRResultShort(&Serial);
    uint16_t command = IrReceiver.decodedIRData.command;
    remoteController.printCommand(command);
    remoteController.handleCommand(command);
  
    IrReceiver.resume();
  }
}

ISR (SPI_STC_vect){ // SPI Tx ISR
  mspIsLive = true;
  elapsedSeconds = 0;
  Slavereceived = SPDR; // Take data from buffer sent by MASTER
  Serial.print("Data from Master received: ");
  Serial.println(Slavereceived);
  SPDR = slaveSend; // Send data to buffer to send to MASTER
  analogWrite(PIN_GREEN, 50);
}