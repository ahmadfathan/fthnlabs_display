#ifndef FTHN_LABS_DISPLAY_H
#define FTHN_LABS_DISPLAY_H

#include <Adafruit_GFX.h>
#include "DisplayConfig.h"

#include "Arduino.h"

#include <SPI.h>

#include <cstdlib>

#define TIMER_FREQ 1000000L
#define SCAN_INTERVAL_MS 1 // 1ms

#define SPI_CLK 4000000L

#define USE_DMA true

class FthnLabsDisplay : public Adafruit_GFX
{

public:
    FthnLabsDisplay(DisplayConfig config);

    // Required override
    void drawPixel(int16_t x, int16_t y, uint16_t color) override;

    bool begin();
    void show();
    void loop();
    void setBrightness(uint8_t brightness); // 0 - 100 

private:
    DisplayConfig config;

    SPIClass *vspi = NULL;

    hw_timer_t *timer = NULL;

    uint8_t *frameBuffer8 = NULL;
    uint16_t *frameBuffer16 = NULL;

    uint8_t currentGroup = 0;

    uint8_t _brightness = 100;
    uint8_t _oeOnTime;

    void lightRowOfAddress(uint8_t address)
    {
        for (uint8_t i = 0; i < config.numOfAddressPins; i++)
        {
            digitalWrite(config.addressPins[i], ((address >> i) & 0x01));
        }
    }

    void latchShiftRegToOutput()
    {
        digitalWrite(config.latPin, HIGH);
        digitalWrite(config.latPin, LOW);
    }

    void oeRowsOff()
    {
        digitalWrite(config.oePin, LOW);
    }

    void oeRowsOn()
    {
        digitalWrite(config.oePin, HIGH);
    }

    void calculateOeOnTime() {
        _oeOnTime = map(_brightness, 0, 100, 0, 250);
    }

    void scanDisplay()
    {
        uint8_t bytes;
        uint8_t numOfRows = 4;

        for (uint8_t xOffset = 0; xOffset < 32; xOffset += 8)
        {
            vspi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0));
            for (uint8_t i = 0; i < 4; i++)
            {
                bytes = 0x00;
                for (uint8_t x = 0; x < 8; x++)
                {
                    uint16_t y = (currentGroup + (numOfRows * (3 - i)));
                    uint8_t state = frameBuffer8[y * width() + x + xOffset] == 255;
                    bytes = bytes | state << (7 - x);
                }
                vspi->transfer(~bytes);
            }
            vspi->endTransaction();
        }

        latchShiftRegToOutput();
        lightRowOfAddress(currentGroup);

        oeRowsOn();
        delayMicroseconds(_oeOnTime);
        oeRowsOff();

        currentGroup++;
        if (currentGroup > 3)
        {
            currentGroup = 0;
        }
    }
};

#endif