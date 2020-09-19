#include <iostream>
#include "code.h"
#include "encoding.h"
#include "binaryoutfile.h"

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
  symbolTable.checkConsistency();
}

void Code::backpatch() {
  for (auto& es : encodedSections) {
    const string& section = es.first;

    for (const auto& bp : es.second.symbolLocations) {
      if (!symbolTable.isSymbolDefined(bp.symbol))
        throw AssemblerException("Symbol " + bp.symbol + " is not defined or external to be used");

      Encoding::addBytes(symbolTable.getValue(bp.symbol), bp.size, bp.location, es.second.bytes);
      if (bp.pcRelOffset > 0) {
        Encoding::addBytes(-bp.pcRelOffset, bp.size, bp.location, es.second.bytes);

        if (symbolTable.getReference(bp.symbol) == section) {
          Encoding::addBytes(-bp.location, bp.size, bp.location, es.second.bytes);
        } else {
          relocations.addRelocation(section, R_PC, symbolTable.getReference(bp.symbol), bp.location);
        }
      }
      else if (symbolTable.getType(bp.symbol) != ABS) {
        RelocationType type = (bp.size == 1) ? R_8 : R_16;
        relocations.addRelocation(section, type, symbolTable.getReference(bp.symbol), bp.location);
      }
    }
  }
}

void Code::generateObjectFile(const string& path) const {
  BinaryOutFile outFile(path);

  set<string> symbolsToExport = symbolTable.getExportSymbols();
  outFile.write(ChunkHeader{SYMBOLS, (int)symbolsToExport.size()});
  for (const string& symbol : symbolsToExport)
    outFile.write(symbolTable.getEntry(symbol));

  for (auto& es : encodedSections) {
    const string& section = es.first;

    outFile.write(ChunkHeader{SECTION, (int)es.second.bytes.size()});
    outFile.write(section);
    for (char byte : es.second.bytes)
      outFile.write(byte);
  }

  for (auto& relo : relocations) {
    const string& relForSection = relo.first;

    outFile.write(ChunkHeader{RELOCATION, (int)relo.second.size()});
    outFile.write(relForSection);
    for (const RelEntry& relEntry : relo.second)
      outFile.write(relEntry);
  }
}

void Code::printInfo(ostream& os) const {
  os << endl << "---------------------" << endl;
  os << "SYMBOLS: "<< endl;
  symbolTable.printSymbolTable(os);

  os << endl << "-------------------------------" << endl;
  for (auto &p : encodedSections) {
    os << endl << "SECTION: " << p.first << endl;
    for (auto byte : p.second.getBytes())
      os << byte << " ";
  }
}
