#ifndef MEMORY_H
#define MEMORY_H
#include "typesconstants.h"
#include <cstdint>
#include <vector>
using namespace std;

class Memory {
  vector<char> memory;

public:
  Memory() : memory(MEM_SIZE, 0) {}

  void load(int startLocation, const vector<char> &bytes);

  int16_t read(int location, int size);
  void write(int location, int16_t value, int size);
};

#endif
