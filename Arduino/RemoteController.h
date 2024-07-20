#ifndef REMOTECONTROLLER_H
#define REMOTECONTROLLER_H

#define CMD_ON 64
#define CMD_OFF 65
#define CMD_VOLUME_DOWN 93
#define CMD_VOLUME_UP 92
#define CMD_WHITE 68
#define CMD_BLUE 69
#define CMD_GREEN 89
#define CMD_RED 88
#define CMD_PI1 72
#define CMD_LIGHT_BLUE 73
#define CMD_LIGHT_GREEN 85
#define CMD_LIGHT_RED 84
#define CMD_PI2 76
#define CMD_PURPLE1 77
#define CMD_CYAN 81
#define CMD_ORANGE 80
#define CMD_SKY1 31
#define CMD_LIGHT_PURPLE 30
#define CMD_TEAL2 29
#define CMD_LIGHT_ORANGE 28
#define CMD_SKY2 27
#define CMD_REALLY_LIGHT_PURPLE 26
#define CMD_DARK_GREEN 25
#define CMD_YELLOW 24

#define ON true
#define OFF false

#include <Arduino.h>

class RemoteController {
public:
  RemoteController(int, int, int);

  volatile bool status = OFF;

  void printCommand(uint16_t command);
  void handleCommand(uint16_t command);
private:
  int redPin;
  int greenPin;
  int bluePin;
  void setColor(int redValue, int greenValue, int blueValue);
};

#endif // COMMANDHANDLER_H
