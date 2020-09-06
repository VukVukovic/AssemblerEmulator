#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include "assemblerexception.h"
#include <string>
#include <map>
#include <vector>
#include <set>
using namespace std;

enum SymbolType { EXT, ABS, REL };

class SymbolTable {
public:
  struct STEntry {
    int value = 0;
    string reference;
    SymbolType type;
  };

private:
  map<string, STEntry> symbols;
  set<string> externSymbols;
  set<string> globalSymbols;
  set<string> sections;

  void checkAlreadyDefined(const string& symbol) const;

public:
  void addExtern(const string& symbol);
  void addGlobal(const string& symbol);

  void defineAbsoluteSymbol(const string& symbol, int value);
  void defineRelocatableSymbol(const string& symbol, int value, const string& section);
  void defineExternSymbol(const string& symbol, int value, const string& externSymbol);
  void defineSection(const string& section);

  bool isSymbolDefined(const string& symbol) const { return symbols.find(symbol) != symbols.end(); }
  SymbolType getType(const string& symbol) const { return symbols.at(symbol).type; }
  string getLocalClassification(const string& symbol) const;
  string getReference(const string& symbol) const { return symbols.at(symbol).reference; }
  int getValue(const string& symbol) const { return symbols.at(symbol).value; }

  void includeExtern();

  void printSymbolTable(ostream &out);

  void checkConsistency() const;

  set<string> getExportSymbols() const;
  //map<string, STEntry>::const_iterator begin() const { return symbols.begin(); }
  //map<string, STEntry>::const_iterator end() const { return symbols.end(); }
};

#endif
