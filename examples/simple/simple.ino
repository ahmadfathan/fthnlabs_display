#include <FthnLabsDisplay.h>
#include "config.h"

FthnLabsDisplay display(cfg);

void setup()
{
  Serial.begin(115200);

  if (!display.begin())
  {
    Serial.println("Failed to initialize display");
    while (1)
      ;
  }

  display.setBrightness(BRIGHTNESS);
  display.println("Hello");
}

void loop()
{
  display.loop();
}
