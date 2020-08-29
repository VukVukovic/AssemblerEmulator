#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <string>
#include <map>
#include <vector>
using namespace std;

struct DuplicateSymbols : public exception {
    const char* what() const throw() {
        return "Duplicate symbols!";
    }
};

struct AssemblerException : public exception {
    string problem;

    AssemblerException(const string& problem) : problem(problem) {}

    virtual const char* what() const throw() {
        return "Assembler exception!";
    }

    const string& getProblem() const { return problem; }
};

struct ClassificationIndexInvalid : public AssemblerException {
    using AssemblerException::AssemblerException;

    virtual const char* what() const throw() {
        return "Invalid classification index";
    }
};

struct SymbolsCannotBeResolved : public AssemblerException {
    using AssemblerException::AssemblerException;

    virtual const char* what() const throw() {
        return "Symbols could not be resolved";
    }
};

class SymbolTable {
  // section: /EXT, /ABS
  struct STEntry {
    int value = 0;
    string section;
    bool isExtern = false;
    bool resolved = true;
  };
  map<string, STEntry> symbols;

  struct EQUEntry {
    string symbol;
    int value = 0;
    vector<pair<int, string>> depend;
    bool defined = false;
    bool canBeResolved(const SymbolTable& symbolTable) const;
    void resolve(SymbolTable& symbolTable);
  };

  map<string, EQUEntry> equTable;
  map<string, vector<string>> waiting;

  struct Backpatch {
      string symbol;
      string section;
      int size;
      int offset;
      Backpatch(const string& symbol, const string& section, int size, int offset)
        : symbol(symbol), section(section), size(size), offset(offset) {}
  };
  vector<Backpatch> backpatching;

  void printEquTable(ostream &out);

public:
  void addExtern(const string& symbol);
  void addGlobal(const string& symbol);
  void addLabel(const string& symbol, const string& section, int value);
  void addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols);

  void addUsage(const string& symbol, int size, const string& section, int offset);
  void printSymbolTable(ostream &out);

  void resolveSymbols();
};

#endif
