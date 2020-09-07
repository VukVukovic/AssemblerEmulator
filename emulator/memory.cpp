#include "memory.h"
#include <iostream>
//#include <string.h>

void Memory::load(int startLocation, const vector<char>& bytes) {
  //memcpy(&memory[startLocation], &bytes[0], bytes.size() * sizeof(char));
  copy(bytes.begin(), bytes.end(), memory.begin()+startLocation);
}
