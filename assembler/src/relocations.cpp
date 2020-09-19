#include "relocations.h"

void Relocations::addRelocation(const string& section, RelocationType type, const string& symbol, int offset) {
  relocations[section].push_back(RelEntry(type, symbol, offset));
}
