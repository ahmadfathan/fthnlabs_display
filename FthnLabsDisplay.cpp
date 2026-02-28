#include "FthnLabsDisplay.h"

bool FthnLabsDisplay::_shouldScan = false;

void IRAM_ATTR FthnLabsDisplay::triggerScan()
{
    FthnLabsDisplay::_shouldScan = true;
}

FthnLabsDisplay::FthnLabsDisplay(DisplayConfig config) : Adafruit_GFX(config.w, config.h), config(config)
{
}

// Implementation of required virtual function
void FthnLabsDisplay::drawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || y < 0 || x >= width() || y >= height())
        return;

    drawBuffer->write(x, y, color);
}

bool FthnLabsDisplay::begin()
{
    vspi = new SPIClass(VSPI);

    vspi->begin(config.clkPin, -1, config.dataPins[0], -1);

    uint32_t numOfPixels = config.w * config.h;

    drawBuffer = new Buffer2D(config.w, config.h, BufferType::BUF1);

    // Ceiling division of config.h by 16
    uint32_t dispWidth = ((config.h + 15) / 16) * config.w;

    dispBuffer = new Buffer2D(dispWidth, 16, BufferType::BUF1);

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

    timerAlarm(timer, (TIMER_FREQ * SCAN_INTERVAL_MS) / 1000, true, 0);

    return true;
}

void FthnLabsDisplay::setBrightness(uint8_t brightness)
{
    _brightness = brightness;
    calculateOeOnTime();
}

void FthnLabsDisplay::loop()
{
    if (!FthnLabsDisplay::_shouldScan)
        return;

    scan();
}

void FthnLabsDisplay::display()
{
    // copy & transform drawbuffer to display buffer
    for (uint16_t y = 0; y < height(); y++)
    {
        for (uint16_t x = 0; x < width(); x++)
        {
            uint16_t val = drawBuffer->read(x, y);

            uint16_t dispY = y % 16;
            uint16_t dispX = (y / 16) * 32 + x;

            dispBuffer->write(dispX, dispY, val);
        }
    }
}

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

void FthnLabsDisplay::scan()
{
    uint8_t bytes;
    uint8_t numOfRows = 4;

    uint16_t dispWidth = dispBuffer->width();
    uint16_t txBufferLen = dispWidth / 2;

    uint8_t txBuffer[txBufferLen];
    uint8_t idx = 0;

    // send screen_width x 4 pixels
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 1/2/3/4)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 5/6/7/8)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 9/10/11/12)
    // * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * (row 13/14/15/16)

    for (uint8_t xOffset = 0; xOffset < dispWidth; xOffset += 8)
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

            // pack 8 pixels into one byte
            bytes |= dispBuffer->read(0 + xOffset, y) << 7;
            bytes |= dispBuffer->read(1 + xOffset, y) << 6;
            bytes |= dispBuffer->read(2 + xOffset, y) << 5;
            bytes |= dispBuffer->read(3 + xOffset, y) << 4;
            bytes |= dispBuffer->read(4 + xOffset, y) << 3;
            bytes |= dispBuffer->read(5 + xOffset, y) << 2;
            bytes |= dispBuffer->read(6 + xOffset, y) << 1;
            bytes |= dispBuffer->read(7 + xOffset, y) << 0;

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