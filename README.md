# FthnLabs Display

<img width="320" height="320" alt="fthn_labs" src="https://github.com/user-attachments/assets/d7ba18b0-f63e-4e5b-8c28-1b3f0c81459f" />

An Arduino library for driving dot matrix displays. Currently, only ESP32 is supported.  
This library also supports AdafruitGFX, so display manipulation can be easily done
using AdafruitGFX functionalities.

<p align="center">
  <br>
  <img width="auto" height="48" alt="image" src="https://github.com/user-attachments/assets/0083d60c-acab-4ff2-b642-c4ab10aa8252" />
  &nbsp;&nbsp;
  <img width="auto" height="48" alt="image" src="https://github.com/user-attachments/assets/14e99ae2-8fda-4520-841b-a8a43af4a7b2" />
  &nbsp;&nbsp;
  <img width="auto" height="48" alt="image" src="https://github.com/user-attachments/assets/95f9525b-fe09-45b8-a162-b41bb1b10bcb" />
  <br>
  
</p>

## A. Supported Display

1. HUB12 dot matrix panel.

## B. Getting Started

### 0. Library Installation

This library was already published to Arduino Library Registry. You can find it by searching `FthnLabsDisplay`. Quickly hit the install button to add the library to your computer.

You can also manually install the library. Clone the this repository, then move it to `{Arduino Directory}/libraries`. Then you can include it into your sketch.

`#include <FthnLabsDisplay.h>`

### 1. Wiring Diagram

<img width="1840" height="1758" alt="Wiring Diagram" src="https://github.com/user-attachments/assets/c416f9f1-7ba7-40de-8483-e724d154c984" />

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
