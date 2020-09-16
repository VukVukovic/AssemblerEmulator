#include "memory.h"
#include <iostream>
//#include <string.h>

void Memory::load(int startLocation, const vector<char>& bytes) {
  //memcpy(&memory[startLocation], &bytes[0], bytes.size() * sizeof(char));
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
}
