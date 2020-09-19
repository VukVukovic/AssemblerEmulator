#include <iostream>
#include "symboltable.h"

void SymbolTable::addExtern(const string& symbol) {
  if (globalSymbols.find(symbol) != globalSymbols.end())
    throw AssemblerException(symbol + " cannot be marked both extern and global");
  externSymbols.insert(symbol);
}

void SymbolTable::addGlobal(const string& symbol) {
  if (externSymbols.find(symbol) != externSymbols.end())
    throw AssemblerException(symbol + " cannot be marked both extern and global");
  globalSymbols.insert(symbol);
}

void SymbolTable::printSymbolTable(ostream &out) const {
  string types[3] = {"EXT", "ABS", "REL"};
  for (auto s : symbols) {
    out << s.first << '\t' << s.second.value << '\t' << s.second.reference << '\t' << types[s.second.type] << std::endl;
  }
  out << "----------------------------" << endl;
}

void SymbolTable::includeExtern() {
  for (const string& externSymbol : externSymbols) {
    checkAlreadyDefined(externSymbol);
    symbols[externSymbol] = SymbolEntry(externSymbol, 0, externSymbol, EXT);
  }
}

void SymbolTable::defineAbsoluteSymbol(const string& symbol, int value) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = SymbolEntry(symbol, value, "", ABS);
}

void SymbolTable::defineRelocatableSymbol(const string& symbol, int value, const string& section) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = SymbolEntry(symbol, value, section, REL);
}

void SymbolTable::defineExternSymbol(const string& symbol, int value, const string& externSymbol) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = SymbolEntry(symbol, value, externSymbol, EXT);
}

void SymbolTable::defineSection(const string& section) {
  defineRelocatableSymbol(section, 0, section);
  sections.insert(section);
}

string SymbolTable::getLocalClassification(const string& symbol) const {
  SymbolType type = symbols.at(symbol).type;
  return type == ABS ? "/ABS" : (type == EXT ? "/EXT" : symbols.at(symbol).reference);
}

void SymbolTable::checkAlreadyDefined(const string& symbol) const {
  if (isSymbolDefined(symbol))
    throw AssemblerException(symbol + " already defined");
}

void SymbolTable::checkConsistency() const {
  for (const string& symbol : globalSymbols) {
    if (externSymbols.find(symbol) != externSymbols.end())
      throw AssemblerException("Symbol " + symbol + " cannot be marked as global since it's extern");
    if (sections.find(symbol) != sections.end())
      throw AssemblerException("Sections cannot be marked as global (" + symbol + ")");
  }
}

set<string> SymbolTable::getExportSymbols() const {
  return globalSymbols;
}
