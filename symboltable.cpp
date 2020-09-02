#include <iostream>
#include "symboltable.h"

void SymbolTable::addExtern(const string& symbol) {
  externSymbols.insert(symbol);
}

void SymbolTable::addGlobal(const string& symbol) {
  globalSymbols.insert(symbol);
}

void SymbolTable::addLabel(const string& symbol, const string& section, int value) {
  symbols[symbol] = {value, section};
}

void SymbolTable::addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols) {
  // handle duplicate in both cases
  if (usedSymbols.size() == 0) {
    symbols[equSymbol] = {value, "/ABS"};
  }
  else {
    equTable[equSymbol] = EQUEntry{};
    equTable[equSymbol].value = value;
    equTable[equSymbol].symbol = equSymbol;
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
    out << s.first << '\t' << s.second.value << '\t' << s.second.section <<std::endl;
  }
  out << "----------------------------" << endl;
  printEquTable(out);

}

void SymbolTable::checkExternGlobal() const {
  set<string> intersect;
  set_intersection(externSymbols.begin(), externSymbols.end(),
                    globalSymbols.begin(),globalSymbols.end(),
                    inserter(intersect,intersect.begin()));
  if (intersect.size() > 0) {
    string problematicSymbols = "";
    for (const string& s : intersect) {
      if (problematicSymbols.length() > 0)
        problematicSymbols += ", ";
      problematicSymbols += s;
    }
    throw InvalidSymbolDirective(problematicSymbols + " cannot be marked both extern and global");
  }
}

void SymbolTable::resolveExtern() {
  for (const string& externSymbol : externSymbols) {
    if (symbols.find(externSymbol) != symbols.end())
      throw InvalidSymbolDirective("Symbol " + externSymbol + " cannot be marked extern");
    symbols[externSymbol] = {0, "/EXT"};
  }
}

void SymbolTable::resolveSymbols() {
  checkExternGlobal();
  resolveExtern();

  for (const auto& equRow : equTable) {
    if (symbols.find(equRow.first)==symbols.end())
      if (equTable[equRow.first].canBeResolved(*this)) {
        equTable[equRow.first].resolve(*this);
      }
  }

  string problematic;
  bool problem = false;
  for (const auto& equRow : equTable)
    if (symbols.find(equRow.first)==symbols.end()) {
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
    canResolve &= (symbolTable.symbols.find(p.second) != symbolTable.symbols.end());
  return canResolve;
}

void SymbolTable::EQUEntry::resolve(SymbolTable& symbolTable) {
  map<string, int> classificationIndex;

  for (const auto& p : depend) {
    if (symbolTable.symbols[p.second].section == "/EXT" && p.first == -1)
      throw ClassificationIndexInvalid("Problem with symbol " + symbol + " - " + p.second + " is not allowed");
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

  int valueAdd = 0; string section = "/ABS";
  for (const auto& p : depend) {
    valueAdd += p.first * symbolTable.symbols[p.second].value;
    if (classificationIndex[symbolTable.symbols[p.second].section] == 1)
      section = symbolTable.symbols[p.second].section;
  }

  cout << "Resolved " << symbol << " " << value + valueAdd << " " << section << endl;
  symbolTable.symbols[symbol] = {value + valueAdd, section};

  for (const string& w : symbolTable.waiting[symbol])
    if (symbolTable.equTable[w].canBeResolved(symbolTable))
      symbolTable.equTable[w].resolve(symbolTable);
}

void SymbolTable::printEquTable(ostream &out) {
  for (auto e : equTable) {
    out << e.first << " = ";
    for (auto s : e.second.depend) out << (s.first>0 ? '+' : '-') << s.second << " ";
    out << "\t[";
    for (auto s : waiting[e.first]) out << s << ", ";
    out << "]"<<endl;
  }
}
