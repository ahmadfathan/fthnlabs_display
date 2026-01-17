# FthnLabs Display

An Arduino library for driving dot matrix displays. Currently, only ESP32 is supported.  
This library also supports AdafruitGFX, so display manipulation can be easily done
using AdafruitGFX functionalities.

## A. Supported Display

1. HUB12 dot matrix panel.

## B. Getting Started

### 0. Library Installation

This library was already published to Arduino Library Registry. You can find it by searching `FthnLabsDisplay`. Quickly hit the install button to add the library to your computer.

You can also manually install the library. Clone the this repository, then move it to `{Arduino Directory}/libraries`. Then you can include it into your sketch.

`#include <FthnLabsDisplay.h>`

### 1. Wiring Diagram

<img width="1724" height="1713" alt="Group 1000003043" src="https://github.com/user-attachments/assets/2a678dc8-2398-4db7-831a-c5bfbfb1cd1f" />

To setup a simple example provide in `examples/simple`, at least there must be 3 components, ESP32 Dev Module, IC 74HCT245 (HC can also be used if you are using just few panel), and a P10 dot matrix panel.

You can have a look this code snippet in `config.h` file
```(cpp)
uint8_t oePin = 22;
uint8_t clkPin = 18;
uint8_t latPin = 2;

uint8_t aPin = 19;
uint8_t bPin = 21;

uint8_t rDataPin = 23;
```
You can actually changed the pins if you are using it for another purpose. Pay attention to boot strapping pins, they requires some condition to boot the ESP32 normally. This library utilizes SPI interface to transfer data to the panel, so `rDataPin` cannot be changed, since it is tied to MOSI pin. 

### 2. Running Simple example

Test your setup by uploading a sample code in `examples/simple`. 

## Upcoming Features

1. Animation (fade-in, fade-out, slide-in, slide-out)
2. Running Text

## Support Me

If you found this project useful, you can support me via:

- [Saweria](https://saweria.co/fthnlabs)  
  [![Support Me](https://img.shields.io/badge/Dukung%20Saya-orange?style=flat-square)](https://saweria.co/fthnlabs)

- [Buy Me a Coffee](https://buymeacoffee.com/fthnlabs)  
  [![Buy Me a Coffee](https://img.shields.io/badge/Buy%20Me%20a%20Coffee-ff813f?style=flat-square&logo=buy-me-a-coffee&logoColor=white)](https://buymeacoffee.com/fthnlabs)
