#ifndef BUFFER_H
#define BUFFER_H

#include <stdint.h>

enum BufferType {
  BUF1,   // 1 bit
  BUF8,   // 8 bit
  BUF16,  // 16 bit
};

class Buffer {
public:
  Buffer(uint64_t length);
  Buffer(uint64_t length, BufferType type);
  ~Buffer();
  void write(uint32_t pos, uint16_t data);
  uint16_t read(uint32_t pos);
  uint32_t size();
  void clear();
private:
  uint32_t _length = 0;
  BufferType _type = BUF16;
  uint16_t* _data;

  void write1(uint32_t pos, uint16_t data);
  void write8(uint32_t pos, uint8_t data);
  void write16(uint32_t pos, uint16_t data);

  uint16_t read1(uint32_t pos);
  uint16_t read8(uint32_t pos);
  uint16_t read16(uint32_t pos);

  void init();
};

#endif