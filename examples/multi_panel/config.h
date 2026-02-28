#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>
#include <DisplayConfig.h>

// change this according to panel width and heigh
#define PANEL_WIDTH 64 
#define PANEL_HEIGHT 16

#define BRIGHTNESS 1 // 1 - 100

uint8_t oePin = 22;
uint8_t clkPin = 18;
uint8_t latPin = 2;

uint8_t aPin = 19;
uint8_t bPin = 21;

uint8_t rDataPin = 23;

// address pins
uint8_t addressPins[] = { aPin, bPin };

// data pins
uint8_t dataPins[] = { rDataPin };

DisplayConfig cfg = {
  PANEL_WIDTH, PANEL_HEIGHT,  // panel
  oePin, clkPin, latPin,          // control
  addressPins, 2,                 // address
  dataPins, 1                     // data
};

#endif