#include "buffer.h"
#include <Arduino.h>

Buffer::Buffer(uint64_t length)
  : _length(length) {
  init();
}

Buffer::Buffer(uint64_t length, BufferType type)
  : _length(length), _type(type) {
  init();
}

void Buffer::init() {
  uint32_t bufSize;
  switch (_type) {
    case BUF1:
      bufSize = _length / 16;
      bufSize = bufSize == 0 ? 1 : bufSize;
      break;
    case BUF8:
      bufSize = _length / 2;
      bufSize = bufSize == 0 ? 1 : bufSize;
      break;
    case BUF16:
      bufSize = _length;
      break;
  }

  _data = new uint16_t[bufSize]();
}

Buffer::~Buffer() {
  delete[] _data;
}

void Buffer::write(uint32_t pos, uint16_t data) {
  switch (_type) {
    case BUF1:
      write1(pos, data);
      break;
    case BUF8:
      write8(pos, data);
      break;
    case BUF16:
      write16(pos, data);
      break;
  }
}

void Buffer::write1(uint32_t pos, uint16_t data) {
  uint32_t idx = pos / 16;
  uint8_t mod = pos % 16;

  uint16_t mask = (1u << mod);  // target bit mask

  _data[idx] &= ~mask;                  // clear the bit
  _data[idx] |= ((data & 0x1) << mod);  // set new bit value
}

void Buffer::write8(uint32_t pos, uint8_t data) {
  uint32_t idx = pos / 2;
  uint8_t mod = pos % 2;

  uint16_t mask = 0xFF << (mod * 8);  // target byte mask
  _data[idx] &= ~mask;                // clear that byte
  _data[idx] |= (data << (mod * 8));  // write new value
}

void Buffer::write16(uint32_t pos, uint16_t data) {
  _data[pos] = data;
}

uint16_t Buffer::read(uint32_t pos) {
  switch (_type) {
    case BUF1:
      return read1(pos);
    case BUF8:
      return read8(pos);
    case BUF16:
      return read16(pos);
  }
}

uint16_t Buffer::read1(uint32_t pos) {
  uint32_t idx = pos / 16;
  uint8_t mod = pos % 16;

  return (_data[idx] >> mod) & 0b0000000000000001;
}

uint16_t Buffer::read8(uint32_t pos) {
  uint32_t idx = pos / 2;
  uint8_t mod = pos % 2;

  // return (_data[idx] >> (mod * 8)) & 0xFF;
  return (_data[idx] & (0b0000000011111111 << (mod * 8))) >> (mod * 8);
}

uint16_t Buffer::read16(uint32_t pos) {
  return _data[pos];
}

uint32_t Buffer::size() {
  return _length;
}

void Buffer::clear() {
  memset(_data, 0, _length);
}
