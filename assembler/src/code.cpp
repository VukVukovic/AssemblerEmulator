#include "code.h"
#include "binaryoutfile.h"
#include "encoding.h"
#include <iostream>

const string Code::line(65, '-');

void Code::beginSection(const string &section) {
  symbolTable.defineSection(section);
  currentSection = section;
  encodedSections[currentSection] = Encoding{};
}

void Code::addInstructionDirective(const Encoding &encoding) {
  encodedSections[currentSection].add(encoding);
}

void Code::addExtern(const string &symbol) { symbolTable.addExtern(symbol); }

void Code::addGlobal(const string &symbol) { symbolTable.addGlobal(symbol); }

void Code::addLabel(const string &symbol) {
  symbolTable.defineRelocatableSymbol(
      symbol, encodedSections[currentSection].getBytes().size(),
      currentSection);
}

void Code::addEqu(const string &equSymbol, int value,
                  const vector<pair<int, string>> &usedSymbols) {
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
  for (auto &es : encodedSections) {
    const string &section = es.first;

    for (const auto &bp : es.second.symbolLocations) {
      if (!symbolTable.isSymbolDefined(bp.symbol))
        throw AssemblerException("Symbol " + bp.symbol +
                                 " is not defined or external to be used");

      if (bp.pcRelOffset > 0) {
        Encoding::addBytes(-bp.pcRelOffset, bp.size, bp.location,
                           es.second.bytes);
      }

      if (symbolTable.isSymbolGlobalBinding(bp.symbol)) {
        // global and pure extern symbols are left for linker to handle
        RelocationType type =
            (bp.pcRelOffset > 0) ? R_PC : ((bp.size == 1) ? R_8 : R_16);
        relocations.addRelocation(section, type, bp.symbol, bp.location);
        continue;
      }

      // all other relocation types add symbol value
      Encoding::addBytes(symbolTable.getValue(bp.symbol), bp.size, bp.location,
                         es.second.bytes);

      if (bp.pcRelOffset > 0 &&
          symbolTable.getReference(bp.symbol) == section) {
        // symbol in the same section is resolved
        Encoding::addBytes(-bp.location, bp.size, bp.location, es.second.bytes);
        continue;
      }

      if (symbolTable.getType(bp.symbol) != ABS) {
        RelocationType type =
            (bp.pcRelOffset > 0) ? R_PC : ((bp.size == 1) ? R_8 : R_16);
        relocations.addRelocation(
            section, type, symbolTable.getReference(bp.symbol), bp.location);
      }
    }
  }
}

void Code::generateObjectFile(const string &path) const {
  BinaryOutFile outFile(path);

  set<string> symbolsToExport = symbolTable.getExportSymbols();
  outFile.write(ChunkHeader{SYMBOLS, (int)symbolsToExport.size()});
  for (const string &symbol : symbolsToExport)
    outFile.write(symbolTable.getEntry(symbol));

  for (auto &es : encodedSections) {
    const string &section = es.first;

    outFile.write(ChunkHeader{SECTION, (int)es.second.bytes.size()});
    outFile.write(section);
    for (char byte : es.second.bytes)
      outFile.write(byte);
  }

  for (auto &relo : relocations) {
    const string &relForSection = relo.first;

    outFile.write(ChunkHeader{RELOCATION, (int)relo.second.size()});
    outFile.write(relForSection);
    for (const RelEntry &relEntry : relo.second)
      outFile.write(relEntry);
  }
}

string Code::getHex(char byte) {
  static const char hexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  string s;
  s += hexChars[(byte & 0xF0) >> 4];
  s += hexChars[(byte & 0x0F) >> 0];
  return s;
}

void Code::printInfo(ostream &os) const {
  symbolTable.printSymbolTable(os, line);

  os << endl;
  for (auto &p : encodedSections) {
    if (p.second.getBytes().size() > 0) {
      os << "Section " << p.first << endl;
      os << line << endl;
      int cnt = 0;
      for (auto byte : p.second.getBytes()) {
        os << getHex(byte) << " ";
        if (++cnt % 10 == 0)
          os << endl;
      }
      if (cnt % 10 != 0)
        os << endl;
      os << endl;
    }

    relocations.printRelocaions(p.first, os, line);
  }
}
