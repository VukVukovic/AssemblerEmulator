#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "assemblerexception.h"
#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

enum SymbolType { EXT, ABS, REL };

struct SymbolEntry {
  string symbol;
  int value = 0;
  string reference;
  SymbolType type;
  SymbolEntry() = default;
  SymbolEntry(const string &symbol, int value, const string &reference,
              SymbolType type)
      : symbol(symbol), value(value), reference(reference), type(type) {}
};

class SymbolTable {
  map<string, SymbolEntry> symbols;
  set<string> externSymbols;
  set<string> globalSymbols;
  set<string> sections;

  void checkAlreadyDefined(const string &symbol) const;

public:
  void addExtern(const string &symbol);
  void addGlobal(const string &symbol);

  void defineAbsoluteSymbol(const string &symbol, int value);
  void defineRelocatableSymbol(const string &symbol, int value,
                               const string &section);
  void defineExternSymbol(const string &symbol, int value,
                          const string &externSymbol);
  void defineSection(const string &section);

  bool isSymbolDefined(const string &symbol) const {
    return symbols.find(symbol) != symbols.end();
  }
  bool isSymbolGlobalBinding(const string &symbol) const {
    return globalSymbols.find(symbol) != globalSymbols.end() ||
           externSymbols.find(symbol) != externSymbols.end();
  }
  SymbolType getType(const string &symbol) const {
    return symbols.at(symbol).type;
  }
  string getLocalClassification(const string &symbol) const;
  string getReference(const string &symbol) const {
    return symbols.at(symbol).reference;
  }
  int getValue(const string &symbol) const { return symbols.at(symbol).value; }
  SymbolEntry getEntry(const string &symbol) const {
    return symbols.at(symbol);
  }

  void includeExtern();

  void printSymbolEntry(const SymbolEntry &symbolEntry, ostream &out) const;
  void printSymbolTable(ostream &out, const string &line) const;

  void checkConsistency() const;

  set<string> getExportSymbols() const;
  // map<string, SymbolEntry>::const_iterator begin() const { return
  // symbols.begin(); } map<string, SymbolEntry>::const_iterator end() const {
  // return symbols.end(); }
};

#endif
