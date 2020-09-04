#ifndef CODE_H
#define CODE_H

#include <map>
#include <vector>
#include <string>
#include "symboltable.h"
#include "equtable.h"

using namespace std;
class Encoding;

class Code {
    string currentSection;

public:
    SymbolTable symbolTable;
    EquTable equTable;
    map<string, Encoding> encodedSections;

/*
    struct Backpatch {
        string symbol;
        string section;
        int size;
        int offset;
        Backpatch(const string& symbol, const string& section, int size, int offset)
          : symbol(symbol), section(section), size(size), offset(offset) {}
    };
    vector<Backpatch> backpatching;
*/
    void beginSection(const string& section); // .section
    void addInstructionDirective(const Encoding& encoding); // instructions, .byte, .word, .skip

    void addExtern(const string& symbol); // .extern
    void addGlobal(const string& symbol); // .global
    void addLabel(const string& symbol); // label:

    void addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols); // .equ

    void resolveSymbols();
    void backpatch();
};

#endif
