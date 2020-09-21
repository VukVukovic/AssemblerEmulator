#include "symboltable.h"
#include <iomanip>
#include <iostream>

void SymbolTable::addExtern(const string &symbol) {
  if (globalSymbols.find(symbol) != globalSymbols.end())
    throw AssemblerException(symbol +
                             " cannot be marked both extern and global");
  externSymbols.insert(symbol);
}

void SymbolTable::addGlobal(const string &symbol) {
  if (externSymbols.find(symbol) != externSymbols.end())
    throw AssemblerException(symbol +
                             " cannot be marked both extern and global");
  globalSymbols.insert(symbol);
}

void SymbolTable::printSymbolEntry(const SymbolEntry &se, ostream &out) const {
  string reference = se.reference;
  if (se.type == ABS)
    reference = "ABS";
  if (externSymbols.find(se.symbol) != externSymbols.end())
    reference = "UND";

  char bind = (globalSymbols.find(se.symbol) != globalSymbols.end() ||
               externSymbols.find(se.symbol) != externSymbols.end())
                  ? 'g'
                  : 'l';
  out << left << setw(20) << se.symbol << setw(10) << se.value << setw(20)
      << reference << setw(5) << bind;
  out << endl;
}

void SymbolTable::printSymbolTable(ostream &out, const string &line) const {
  out << "Symbol Table" << endl << line << endl;
  out << left << setw(20) << "Name";
  out << setw(10) << "Value" << setw(20) << "Reference" << setw(5) << "Bind"
      << endl
      << line << endl;
  for (auto &section : sections) {
    printSymbolEntry(symbols.at(section), out);
  }
  for (const auto &p : symbols) {
    if (sections.find(p.first) != sections.end())
      continue;
    printSymbolEntry(p.second, out);
  }
  out << line << endl;
}

void SymbolTable::includeExtern() {
  for (const string &externSymbol : externSymbols) {
    checkAlreadyDefined(externSymbol);
    symbols[externSymbol] = SymbolEntry(externSymbol, 0, externSymbol, EXT);
  }
}

void SymbolTable::defineAbsoluteSymbol(const string &symbol, int value) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = SymbolEntry(symbol, value, "", ABS);
}

void SymbolTable::defineRelocatableSymbol(const string &symbol, int value,
                                          const string &section) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = SymbolEntry(symbol, value, section, REL);
}

void SymbolTable::defineExternSymbol(const string &symbol, int value,
                                     const string &externSymbol) {
  checkAlreadyDefined(symbol);
  symbols[symbol] = SymbolEntry(symbol, value, externSymbol, EXT);
}

void SymbolTable::defineSection(const string &section) {
  defineRelocatableSymbol(section, 0, section);
  sections.insert(section);
}

string SymbolTable::getLocalClassification(const string &symbol) const {
  SymbolType type = symbols.at(symbol).type;
  return type == ABS ? "/ABS"
                     : (type == EXT ? "/EXT" : symbols.at(symbol).reference);
}

void SymbolTable::checkAlreadyDefined(const string &symbol) const {
  if (isSymbolDefined(symbol))
    throw AssemblerException(symbol + " already defined");
}

void SymbolTable::checkConsistency() const {
  for (const string &symbol : globalSymbols) {
    if (!isSymbolDefined(symbol))
      throw AssemblerException("Cannot export undefined symbol " + symbol);
    if (symbols.at(symbol).type == EXT)
      throw AssemblerException(
          "Symbol " + symbol +
          " cannot be marked global since it's extern or depends on extern");
    if (sections.find(symbol) != sections.end())
      throw AssemblerException("Symbol " + symbol +
                               " cannot be marked global since it's a section");
  }
}

set<string> SymbolTable::getExportSymbols() const { return globalSymbols; }
