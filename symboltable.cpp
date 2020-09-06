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

void SymbolTable::printSymbolTable(ostream &out) {
  string types[3] = {"EXT", "ABS", "REL"};
  for (auto s : symbols) {
    out << s.first << '\t' << s.second.value << '\t' << s.second.reference << '\t' << types[s.second.type] << std::endl;
  }
  out << "----------------------------" << endl;
}

void SymbolTable::includeExtern() {
  for (const string& externSymbol : externSymbols) {
    checkAlreadyDefined(externSymbol);
    symbols[externSymbol] = {0, externSymbol, EXT};
  }
}

void SymbolTable::defineAbsoluteSymbol(const string& symbol, int value) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = {value, "", ABS};
}

void SymbolTable::defineRelocatableSymbol(const string& symbol, int value, const string& section) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = {value, section, REL};
}

void SymbolTable::defineExternSymbol(const string& symbol, int value, const string& externSymbol) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = {value, externSymbol, EXT};
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
    if (symbols.at(symbol).type == EXT)
      throw AssemblerException("Symbol " + symbol + " cannot be marked as global since it's extern");
  }
}

set<string> SymbolTable::getExportSymbols() const {
  set<string> symbolsToExport(sections);
  symbolsToExport.insert(globalSymbols.begin(), globalSymbols.end());
  return symbolsToExport;
}
