#include "memory.h"
#include <iostream>
//#include <string.h>

void Memory::load(int startLocation, const vector<char>& bytes) {
  //memcpy(&memory[startLocation], &bytes[0], bytes.size() * sizeof(char));
  copy(bytes.begin(), bytes.end(), memory.begin()+startLocation);
}

int8_t Memory::readByte(int location) {
  return memory[location];
}

int16_t Memory::readWord(int location) {
  return (memory[location+1] << 8) | memory[location];
}

void Memory::writeByte(int location, int8_t value) {
  memory[location] = value;
}

void Memory::writeWord(int location, int16_t value) {
  *(&memory[location]) = value;
}
