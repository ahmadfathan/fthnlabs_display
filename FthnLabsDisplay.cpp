#include "FthnLabsDisplay.h"

static bool shouldScan = false;

void IRAM_ATTR triggerScan()
{
    shouldScan = true;
}

FthnLabsDisplay::FthnLabsDisplay(DisplayConfig config) : Adafruit_GFX(config.w, config.h), config(config) {};

// Implementation of required virtual function
void FthnLabsDisplay::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || y < 0 || x >= width() || y >= height())
        return;

    frameBuffer8[y * width() + x] = color;
}

bool FthnLabsDisplay::begin()
{
    vspi = new SPIClass(VSPI);

    vspi->begin(config.clkPin, -1, config.dataPins[0], -1);

    uint32_t numOfPixels = config.w * config.h;

    if (USE_DMA)
    {
        frameBuffer8 = (uint8_t *)heap_caps_malloc(numOfPixels, MALLOC_CAP_DMA);
    }
    else
    {
        frameBuffer8 = (uint8_t *)malloc(numOfPixels);
    }

    if (!frameBuffer8)
        return false;

    memset(frameBuffer8, 0, numOfPixels);

    // Setup pin mode
    for (uint8_t i = 0; i < config.numOfAddressPins; i++)
    {
        pinMode(config.addressPins[i], OUTPUT);
    }
    pinMode(config.oePin, OUTPUT);
    pinMode(config.latPin, OUTPUT);

    // Setup initial GPIO state
    for (uint8_t i = 0; i < config.numOfAddressPins; i++)
    {
        digitalWrite(config.addressPins[i], LOW);
    }

    digitalWrite(config.latPin, LOW);
    digitalWrite(config.oePin, LOW);

    calculateOeOnTime();

    timer = timerBegin(TIMER_FREQ);

    // Attach interrupt to the timer
    timerAttachInterrupt(timer, triggerScan);

    timerAlarm(timer, ((TIMER_FREQ * SCAN_INTERVAL_MS) / 1000), true, 0);

    return true;
}

void FthnLabsDisplay::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    calculateOeOnTime();
}

void FthnLabsDisplay::loop()
{
    if (!shouldScan)
        return;

    scanDisplay();
}

void FthnLabsDisplay::show() {}

void FthnLabsDisplay::lightRowOfAddress(uint8_t address)
{
    for (uint8_t i = 0; i < config.numOfAddressPins; i++)
    {
        digitalWrite(config.addressPins[i], ((address >> i) & 0x01));
    }
}

void FthnLabsDisplay::latchShiftRegToOutput()
{
    digitalWrite(config.latPin, HIGH);
    digitalWrite(config.latPin, LOW);
}

void FthnLabsDisplay::oeRowsOff()
{
    digitalWrite(config.oePin, LOW);
}

void FthnLabsDisplay::oeRowsOn()
{
    digitalWrite(config.oePin, HIGH);
}

void FthnLabsDisplay::calculateOeOnTime()
{
    _oeOnTime = map(_brightness, 0, 100, 0, 250);
}

void FthnLabsDisplay::scanDisplay()
{
    uint8_t bytes;
    uint8_t numOfRows = 4;

    uint8_t txBuffer[16];
    uint8_t idx = 0;

    // send 32x4 pixels
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 1/2/3/4)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 5/6/7/8)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 9/10/11/12)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 13/14/15/16)

    for (uint8_t xOffset = 0; xOffset < 32; xOffset += 8)
    {
        // send 8x4 pixels
        // * * * * * * * *
        // * * * * * * * *
        // * * * * * * * *
        // * * * * * * * *

        for (uint8_t i = 0; i < 4; i++)
        {
            // send 8x1 pixels
            // * * * * * * * *
            bytes = 0x00;

            // precompute row index
            uint16_t y = currentGroup + (numOfRows * (3 - i));
            uint16_t rowBase = y * width() + xOffset;

            // pack 8 pixels into one byte
            bytes |= (frameBuffer8[rowBase + 0] >> 7) << 7;
            bytes |= (frameBuffer8[rowBase + 1] >> 7) << 6;
            bytes |= (frameBuffer8[rowBase + 2] >> 7) << 5;
            bytes |= (frameBuffer8[rowBase + 3] >> 7) << 4;
            bytes |= (frameBuffer8[rowBase + 4] >> 7) << 3;
            bytes |= (frameBuffer8[rowBase + 5] >> 7) << 2;
            bytes |= (frameBuffer8[rowBase + 6] >> 7) << 1;
            bytes |= (frameBuffer8[rowBase + 7] >> 7) << 0;

            // why not using for-loop?
            // - eliminate loop control overhead
            // - better compiler optimization

            txBuffer[idx++] = ~bytes;
        }
    }

    vspi->beginTransaction(SPISettings(SPI_CLK, MSBFIRST, SPI_MODE0));
    vspi->transferBytes(txBuffer, nullptr, idx);
    vspi->endTransaction();

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