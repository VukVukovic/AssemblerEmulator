#include <iostream>
#include "code.h"
#include "encoding.h"

void Code::beginSection(const string& section) {
    symbolTable.defineSection(section);
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
  symbolTable.defineRelocatableSymbol(symbol, encodedSections[currentSection].getBytes().size(), currentSection);
}

void Code::addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols) {
  if (usedSymbols.size() == 0)
    symbolTable.defineAbsoluteSymbol(equSymbol, value);
  else
    equTable.addEqu(equSymbol, value, usedSymbols);
}

void Code::resolveSymbols() {
  symbolTable.includeExtern();
  equTable.resolveSymbols(symbolTable);
}
