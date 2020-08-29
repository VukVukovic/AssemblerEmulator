#include <iostream>
#include "symboltable.h"

void SymbolTable::addExtern(const string& symbol) {
  symbols[symbol] = {0, "/EXT", true, true};
}

void SymbolTable::addGlobal(const string& symbol) {
  symbols[symbol] = STEntry();
}

void SymbolTable::addLabel(const string& symbol, const string& section, int value) {
  symbols[symbol] = {value, section, false, true};
}

void SymbolTable::addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols) {
  // handle duplicate in both cases
  if (usedSymbols.size() == 0) {
    symbols[equSymbol] = {value, "/ABS", false, true};
  }
  else {
    equTable[equSymbol] = EQUEntry{};
    equTable[equSymbol].depend = usedSymbols;
    for (auto us : usedSymbols)
      waiting[us.second].push_back(equSymbol);
  }
}

void SymbolTable::addUsage(const string& symbol, int size, const string& section, int offset) {
  backpatching.push_back(Backpatch(symbol, section, size, offset));
}

void SymbolTable::printSymbolTable(ostream &out) {
  for (auto s : symbols) {
    out << s.first << std::endl;
  }
  out << "----------------------------" << endl;
  printEquTable(out);

}

void SymbolTable::resolveSymbols() {
  for (const auto& equRow : equTable) {
    if (symbols.find(equRow.first)==symbols.end() || !symbols[equRow.first].resolved)
      if (equTable[equRow.first].canBeResolved(*this))
        equTable[equRow.first].resolve(*this);
  }

  string problematic;
  bool problem = false;
  for (const auto& equRow : equTable)
    if (symbols.find(equRow.first)==symbols.end() || !symbols[equRow.first].resolved) {
      if (problem)
        problematic += ", ";
      problematic += equRow.first;
      problem = true;
    }

  if (problem)
    throw SymbolsCannotBeResolved(problematic);

}

bool SymbolTable::EQUEntry::canBeResolved(const SymbolTable& symbolTable) const {
  bool canResolve = true;
  for (const auto& p : depend)
    canResolve &= (symbolTable.symbols.find(p.second) != symbolTable.symbols.end() && symbolTable.symbols.at(p.second).resolved);
  return canResolve;
}

void SymbolTable::EQUEntry::resolve(SymbolTable& symbolTable) {
  map<string, int> classificationIndex;

  for (const auto& p : depend) {
    int index = p.first;
    if (symbolTable.symbols[p.second].section == "/EXT") index = 1;
    else if (symbolTable.symbols[p.second].section == "/ABS") index = 0;
    classificationIndex[symbolTable.symbols[p.second].section] += index;
  }

  bool valid = true, found1 = false;
  for (const auto& ci : classificationIndex) {
    if (ci.second < 0 || ci.second > 1) valid = false;
    else if (ci.second == 1) { valid = !found1; found1 = true; }

    if (!valid)
      throw ClassificationIndexInvalid("Problem with symbol " + symbol + " and classification index of " + ci.first);
  }

  int value = 0; string section = "/ABS";
  for (const auto& p : depend) {
    value += p.first * symbolTable.symbols[p.second].value;
    if (classificationIndex[symbolTable.symbols[p.second].section] == 1)
      section = symbolTable.symbols[p.second].section;
  }

  symbolTable.symbols[symbol] = {value, section, false, true};

  for (const string& w : symbolTable.waiting[symbol])
    if (symbolTable.equTable[w].canBeResolved(symbolTable))
      symbolTable.equTable[w].resolve(symbolTable);
}

void SymbolTable::printEquTable(ostream &out) {
  for (auto e : equTable) {
    out << e.first << " = ";
    for (auto s : e.second.depend) out << (s.first ? '+' : '-') << s.second << " ";
    out << "\t[";
    for (auto s : waiting[e.first]) out << s << ", ";
    out << "]"<<endl;
  }
}
