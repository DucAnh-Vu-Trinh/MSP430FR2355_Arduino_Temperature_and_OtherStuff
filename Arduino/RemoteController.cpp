#include "RemoteController.h"

RemoteController::RemoteController(int redPin, int greenPin, int bluePin)
  : redPin(redPin), greenPin(greenPin), bluePin(bluePin){}

void RemoteController::printCommand(uint16_t command) {
  switch (command) {
    case CMD_ON: 
        Serial.println("ON"); 
        break;
    case CMD_OFF:
        Serial.println("OFF");
        break;
    case CMD_VOLUME_DOWN: 
        Serial.println("V-"); 
        break;
    case CMD_VOLUME_UP:
        Serial.println("V+");
        break;    
    case CMD_WHITE: 
        Serial.println("W"); 
        break;
    case CMD_BLUE:
        Serial.println("B");
        break;        
    case CMD_GREEN:
        Serial.println("G");
        break;
    case CMD_RED:
        Serial.println("R");
        break;
    case CMD_PI1:
        Serial.println("PI1");
        break;
    case CMD_LIGHT_BLUE:
        Serial.println("LIGHT B");
        break;
    case CMD_LIGHT_GREEN:
        Serial.println("LIGHT G");
        break;
    case CMD_LIGHT_RED:
        Serial.println("LIGHT R");
        break;
    case CMD_PI2:
        Serial.println("PI2");
        break;
    case CMD_PURPLE1:
        Serial.println("PUR1");
        break;
    case CMD_CYAN:
        Serial.println("CYAN");
        break;
    case CMD_ORANGE:
        Serial.println("ORANGE");
        break;
    case CMD_SKY1:
        Serial.println("SKY1");
        break;
    case CMD_LIGHT_PURPLE:
        Serial.println("LIGHT PUR");
        break;
    case CMD_TEAL2:
        Serial.println("TEAL2");
        break;
    case CMD_LIGHT_ORANGE:
        Serial.println("LIGHT ORA");
        break;
    case CMD_SKY2:
        Serial.println("SKY2");
        break;
    case CMD_REALLY_LIGHT_PURPLE:
        Serial.println("REALLY LIGHT PURP");
        break;
    case CMD_DARK_GREEN:
        Serial.println("DARK G");
        break;
    case CMD_YELLOW:
        Serial.println("Y");
        break;    
    default:    
        Serial.println("UNDEFINED");
  }
}

void RemoteController::setColor(int redValue, int greenValue, int blueValue){
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void RemoteController::handleCommand(uint16_t command){
  if (command == CMD_ON){
    setColor(10, 10, 10);
    status = ON;
  }
  else if (command == CMD_OFF){
    setColor(0, 0, 0);
    status = OFF;
    Serial.print("Remote Control: OFF");
  }

  if (status == ON){
    switch (command){
      case CMD_RED:
        setColor(100, 0, 0);
        break;
      case CMD_GREEN:
        setColor(0, 100, 0);
        break;
      case CMD_BLUE:
        setColor(0, 0, 100);
        break;
      case CMD_WHITE:
        setColor(100, 100, 100);
        break;
      case CMD_YELLOW:
        setColor(100, 100, 0);
        break;
      case CMD_CYAN:
        setColor(0, 100, 100);
        break;
      case CMD_ORANGE:
        setColor(225, 165, 0);
        break;
      default:
        continue;
    }
  }
}