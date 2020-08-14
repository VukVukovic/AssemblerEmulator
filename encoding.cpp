#include "encoding.h"

void Encoding::add(const Encoding& encoding) {
    bytes.insert(bytes.end(), encoding.bytes.begin(), encoding.bytes.end());
    symbolLocations.insert(symbolLocations.end(), encoding.symbolLocations.begin(), encoding.symbolLocations.end());
}