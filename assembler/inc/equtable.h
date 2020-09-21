#ifndef EQUTABLE_H
#define EQUTABLE_H

#include <map>
#include <set>
#include <string>
#include <vector>
using namespace std;

class SymbolTable;

class EquTable {
  struct EQUEntry {
    string symbol;
    int value = 0;
    vector<pair<int, string>> depend;

    bool canBeResolved(const SymbolTable &symbolTable) const;
    void resolve(SymbolTable &symbolTable, EquTable &equTable);

    EQUEntry() = default;
    EQUEntry(const string &symbol, int value,
             const vector<pair<int, string>> &depend)
        : symbol(symbol), value(value), depend(depend) {}
  };

  map<string, EQUEntry> equTable;
  map<string, vector<string>> waiting;
  set<string> finished;

  bool isResolved(const string &symbol) const {
    return finished.find(symbol) != finished.end();
  }
  void markResolved(const string &symbol) { finished.insert(symbol); }

public:
  void resolveSymbols(SymbolTable &symbolTable);
  void addEqu(const string &equSymbol, int value,
              const vector<pair<int, string>> &usedSymbols);
  void printEquTable(ostream &out) const;
};
#endif
