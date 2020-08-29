#include "encoding.h"

void Encoding::addBytes(int value, int size, vector<char>& bytes) {
    for (int i = 0; i < size; i++)
        bytes.push_back((char)( (value>>(8*i)) & 0xFF)); // little endian
}

void Encoding::add(const Encoding& encoding) {
    int offset = bytes.size();
    bytes.insert(bytes.end(), encoding.bytes.begin(), encoding.bytes.end());
    for (SymbolEncodingEntry sse : encoding.symbolLocations) {
      sse.location += offset;
      symbolLocations.push_back(sse);
    }
}
