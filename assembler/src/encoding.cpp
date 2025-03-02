#include "encoding.h"
#include <cstdint>
#include <iostream>
#include <string>
using namespace std;

void Encoding::appendBytes(int value, int size, vector<char> &bytes) {
  for (int i = 0; i < size; i++)
    bytes.push_back((char)((value >> (8 * i)) & 0xFF)); // little endian
}

void Encoding::addBytes(int value, int size, int offset, vector<char> &bytes) {
  int16_t currValue;
  if (size == 1)
    currValue = bytes[offset];
  else {
    currValue = *(int16_t *)&bytes[offset];
  }

  currValue += value;

  if (size == 1)
    bytes[offset] = currValue;
  else {
    *(int16_t *)&bytes[offset] = currValue;
  }
}

void Encoding::add(const Encoding &encoding) {
  int offset = bytes.size();
  bytes.insert(bytes.end(), encoding.bytes.begin(), encoding.bytes.end());
  for (const SymbolEncodingEntry &sse : encoding.symbolLocations) {
    symbolLocations.push_back(sse);
    symbolLocations.back().location += offset;
  }
}
