#include <FthnLabsDisplay.h>
#include "config.h"

FthnLabsDisplay display(cfg);

uint8_t brightness = 1;

void setup()
{
  Serial.begin(115200);

  if (!display.begin())
  {
    Serial.println("Failed to initialize display");
    while (1)
      ;
  }

  display.setBrightness(brightness);
  display.println("Hello");
}

void loop()
{
  display.loop();
}
