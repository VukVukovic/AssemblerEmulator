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

void Code::backpatch() {
  for (auto& es : encodedSections) {
    const string& section = es.first;

    for (const auto& bp : es.second.symbolLocations) {
      if (!symbolTable.isSymbolDefined(bp.symbol))
        throw AssemblerException("Symbol " + bp.symbol + " is not defined or external to be used");

      Encoding::addBytes(symbolTable.getValue(bp.symbol), bp.size, bp.location, es.second.bytes); // ABS finished
      if (bp.pcRelOffset > 0) {
        Encoding::addBytes(-bp.pcRelOffset, bp.size, bp.location, es.second.bytes); // add ofset for pc next instruction
        // to do; handle different cases
        cout << "PC_REL relocation for "<< bp.symbol << endl;
      }
      else if (symbolTable.getType(bp.symbol) != ABS) {
        cout << "Using " << bp.symbol << ": REL of size " << 8*bp.size << " for " << symbolTable.getReference(bp.symbol) << endl;
      }
    }
    es.second.symbolLocations.clear();
  }
}
