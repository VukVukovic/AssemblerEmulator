#include "equtable.h"
#include "symboltable.h"
#include <iostream>
#include <string>
using namespace std;

void EquTable::resolveSymbols(SymbolTable& symbolTable) {
  for (const auto& equRow : equTable) {
    if (equTable[equRow.first].canBeResolved(symbolTable)) {
      equTable[equRow.first].resolve(symbolTable, *this);
    }
  }

  bool problem = false;
  string problematic;
  for (const auto& equRow : equTable) {
    if (!symbolTable.isSymbolDefined(equRow.first)) {
      problem = true;
      if (problematic.size()>0)
        problematic += ", ";
      problematic += equRow.first;
    }
  }

  if (problem)
    throw AssemblerException("Symbols cannot be resovled: " + problematic + " (undefined or circular reference)");
}

void EquTable::addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols) {
  equTable[equSymbol] = EQUEntry(equSymbol, value, usedSymbols);

  for (auto us : usedSymbols)
    waiting[us.second].push_back(equSymbol);
}

void EquTable::printEquTable(ostream &out) const {
  for (auto e : equTable) {
    out << e.first << " = ";
    for (auto s : e.second.depend) out << (s.first>0 ? '+' : '-') << s.second << " ";
    out << endl;
  }
}

bool EquTable::EQUEntry::canBeResolved(const SymbolTable& symbolTable) const {
  bool canResolve = true;
  for (const auto& p : depend)
    canResolve &= symbolTable.isSymbolDefined(p.second);
  return canResolve;
}

void EquTable::EQUEntry::resolve(SymbolTable& symbolTable, EquTable& equTable) {
  map<string, int> classificationIndex;

  for (const auto& p : depend) {
    if (symbolTable.getType(p.second) == EXT && p.first == -1)
      throw AssemblerException("Classification index problem with symbol " + symbol + "; - " + p.second + " is not allowed");

    int index = p.first;
    if (symbolTable.getType(p.second) == EXT) index = 1;
    else if (symbolTable.getType(p.second) == ABS) index = 0;
    classificationIndex[symbolTable.getLocalClassification(p.second)] += index;
  }

  bool valid = true, found1 = false;
  for (const auto& ci : classificationIndex) {
    if (ci.second < 0 || ci.second > 1) valid = false;
    else if (ci.second == 1) { valid = !found1; found1 = true; }

    if (!valid)
      throw AssemblerException("Classification index problem with symbol " + symbol + " (index of " + ci.first + ")");
  }

  int valueAdd = 0;
  for (const auto& p : depend)
    valueAdd += p.first * symbolTable.getValue(p.second);

  SymbolType type = ABS; string reference;
  for (const auto& p : depend) {
    if (classificationIndex[symbolTable.getLocalClassification(p.second)] == 1) {
      reference = symbolTable.getReference(p.second);
      type = symbolTable.getType(p.second);
      break;
    }
  }

  switch (type) {
    case ABS: symbolTable.defineAbsoluteSymbol(symbol, value + valueAdd); break;
    case EXT: symbolTable.defineExternSymbol(symbol, value + valueAdd, reference); break;
    case REL: symbolTable.defineRelocatableSymbol(symbol, value + valueAdd, reference); break;
  }

  cout << "RESOLVED " << symbol << endl;
  if (equTable.waiting.find(symbol) != equTable.waiting.end()) {
    for (const string& w : equTable.waiting.at(symbol)) {
      if (equTable.equTable.at(w).canBeResolved(symbolTable))
        equTable.equTable.at(w).resolve(symbolTable, equTable);
    }
  }
}
