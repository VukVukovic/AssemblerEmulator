#include "memory.h"
#include <iostream>

void Memory::load(int startLocation, const vector<char>& bytes) {
  copy(bytes.begin(), bytes.end(), memory.begin()+startLocation);
}

int16_t Memory::read(int location, int size) {
  if (size == 1)
    return memory[location];
  return (memory[location+1] << 8) | memory[location];
}

void Memory::write(int location, int16_t value, int size) {
  if (size == 1)
    memory[location] = value;
  else
    *(&memory[location]) = value;

  if (location == DATA_OUT) {
    // simulate terminal output
    cout << (char)value;
  }
}
