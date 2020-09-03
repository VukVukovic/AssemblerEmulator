#ifndef EQUTABLE_H
#define EQUTABLE_H

#include <string>
#include <map>
#include <vector>
using namespace std;

class SymbolTable;

class EquTable {
  struct EQUEntry {
    string symbol;
    int value = 0;
    vector<pair<int, string>> depend;

    bool canBeResolved(const SymbolTable& symbolTable) const;
    void resolve(SymbolTable& symbolTable, EquTable& equTable);

    EQUEntry() = default;
    EQUEntry(const string& symbol, int value, const vector<pair<int, string>>& depend)
      : symbol(symbol), value(value), depend(depend) {}
  };

  map<string, EQUEntry> equTable;
  map<string, vector<string>> waiting;

public:
  void resolveSymbols(SymbolTable& symbolTable);
  void addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols);
  void printEquTable(ostream &out) const;
};
#endif
