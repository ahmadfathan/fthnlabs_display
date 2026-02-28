#ifndef BUFFER_2D_H
#define BUFFER_2D_H

#include "buffer.h"

class Buffer2D : public Buffer {
public:
  Buffer2D(uint32_t width, uint32_t height, BufferType type);

  void write(uint32_t x, uint32_t y, uint16_t data);
  uint16_t read(uint32_t x, uint32_t y);
  uint32_t width();
  uint32_t height();

private:
  uint32_t _width;
  uint32_t _height;
};

#endif