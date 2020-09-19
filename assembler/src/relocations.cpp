#include "relocations.h"
#include <iostream>
#include <iomanip>

const string Relocations::StringRelocationTypes[3] = {"R_386_8",  "R_386_16", "R_386_PC16"};

void Relocations::addRelocation(const string& section, RelocationType type, const string& symbol, int offset) {
  relocations[section].push_back(RelEntry(type, symbol, offset));
}

void Relocations::printRelocaions(const string& section, ostream& out, const string& line) const {
  if (relocations.find(section) == relocations.end()) return;

  out << "Relocations for "<< section << endl << line << endl;
  out << left << setw(10) << "Offset";
  out << setw(20) << "Type";
  out << setw(20) << "Referenced";
  out << endl << line << endl;

  for (const auto& relo : relocations.at(section)) {
    out << setw(10) << relo.offset;
    out << setw(20) << StringRelocationTypes[relo.type];
    out << setw(20) << relo.symbol;
    out << endl;
  }
}
