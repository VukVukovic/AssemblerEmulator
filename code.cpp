#include <iostream>
#include "code.h"
#include "encoding.h"

void Code::beginSection(const string& section) {
    if (encodedSections.find(section) != encodedSections.end())
      throw DuplicateSection{};

    currentSection = section;
    encodedSections[currentSection] = Encoding{};
}

void Code::addInstructionDirective(const Encoding& encoding) {
    encodedSections[currentSection].add(encoding);
    for (auto sl : encoding.getSymbolLocations()) {
      symbolTable.addUsage(sl.symbol, sl.size, currentSection, sl.location);
    }
}
void Code::addExtern(const string& symbol) {
  symbolTable.addExtern(symbol);
}

void Code::addGlobal(const string& symbol) {
  symbolTable.addGlobal(symbol);
}

void Code::addLabel(const string& symbol) {
  symbolTable.addLabel(symbol, currentSection, encodedSections[currentSection].getBytes().size());
}

void Code::addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols) {
  symbolTable.addEqu(equSymbol, value, usedSymbols);
}
