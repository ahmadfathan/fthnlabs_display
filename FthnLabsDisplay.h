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

    void lightRowOfAddress(uint8_t address);
    void latchShiftRegToOutput();
    void oeRowsOff();
    void oeRowsOn();
    void calculateOeOnTime();
    void scanDisplay();
};

#endif