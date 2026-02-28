#include <FthnLabsDisplay.h>
#include "config.h"
#include <Fonts/FreeSerif9pt7b.h>

FthnLabsDisplay display(cfg);

int16_t posX;
uint64_t lastScrollT;

const char* text = "Thank you for using FTHNLabs display!";
int16_t textX, textY;
uint16_t textW, textH;

uint16_t scrollingIntervalMs = 50;

void setup() {
  Serial.begin(115200);

  if (!display.begin()) {
    Serial.println("Failed to initialize display");
    while (1)
      ;
  }

  display.setTextWrap(false);

  display.setFont(&FreeSerif9pt7b);
  display.getTextBounds(text, 0, 0, &textX, &textY, &textW, &textH);

  display.setBrightness(BRIGHTNESS);

  // set x position to the right of the panel
  posX = display.width();

  lastScrollT = millis();
}

void loop() {
  display.loop();

  if (millis() - lastScrollT > scrollingIntervalMs) {
    lastScrollT = millis();

    display.fillScreen(0);

    display.setCursor(posX, 13);
    display.println(text);
    display.display();

    posX--;

    if (posX + textW + display.width() < 0) {
      posX = display.width();
    }
  }
}
