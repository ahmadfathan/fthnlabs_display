#ifndef DISPLAY_CONFIG_H
#define DISPLAY_CONFIG_H

#include "stdint.h"
#include <stdio.h>

struct DisplayConfig
{
    uint16_t w, h;

    uint8_t oePin;
    uint8_t clkPin;
    uint8_t latPin;

    uint8_t *addressPins;
    uint8_t numOfAddressPins;

    uint8_t *dataPins;
    uint8_t numOfDataPins;
};

#endif
