#ifndef MEMORY_H
#define MEMORY_H
#include <cstdint>
#include <vector>
#include "typesconstants.h"
using namespace std;

class Memory {
  vector<char> memory;

public:
  Memory() : memory(MEM_SIZE, 0) {}

  void load(int startLocation, const vector<char>& bytes);

  int8_t readByte(int location);
  int16_t readWord(int location);
  void writeByte(int location, int8_t value);
  void writeWord(int location, int16_t value);
};

#endif
