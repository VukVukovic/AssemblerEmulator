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
private:
  struct STEntry {
    int value = 0;
    string reference;
    SymbolType type;
  };
  map<string, STEntry> symbols;
  set<string> externSymbols;
  set<string> globalSymbols;
  set<string> sections;

  struct Backpatch {
      string symbol;
      string section;
      int size;
      int offset;
      Backpatch(const string& symbol, const string& section, int size, int offset)
        : symbol(symbol), section(section), size(size), offset(offset) {}
  };
  vector<Backpatch> backpatching;

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

  void addUsage(const string& symbol, int size, const string& section, int offset);
  void printSymbolTable(ostream &out);

  void resolveSymbols();
};

#endif
