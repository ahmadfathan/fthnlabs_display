#include "buffer2d.h"

Buffer2D::Buffer2D(uint32_t width, uint32_t height, BufferType type)
    : Buffer(width * height, type), _width(width), _height(height) {}

void Buffer2D::write(uint32_t x, uint32_t y, uint16_t data)
{
    if (x >= _width || y >= _height)
        return; // prevent overflow
    uint32_t pos = y * _width + x;
    Buffer::write(pos, data);
}

uint16_t Buffer2D::read(uint32_t x, uint32_t y)
{
    if (x >= _width || y >= _height)
        return 0; // prevent overflow
    uint32_t pos = y * _width + x;
    return Buffer::read(pos);
}

uint32_t Buffer2D::width()
{
    return _width;
}

uint32_t Buffer2D::height()
{
    return _height;
}