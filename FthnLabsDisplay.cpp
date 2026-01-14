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