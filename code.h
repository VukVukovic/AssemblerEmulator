#ifndef CODE_H
#define CODE_H

#include <map>
#include <vector>
#include <string>
#include "symboltable.h"
#include "equtable.h"
#include "encoding.h"
#include "relocations.h"

using namespace std;

class Code {
    string currentSection;

public:
    SymbolTable symbolTable;
    EquTable equTable;
    Relocations relocations;
    map<string, Encoding> encodedSections;

    void beginSection(const string& section); // .section
    void addInstructionDirective(const Encoding& encoding); // instructions, .byte, .word, .skip

    void addExtern(const string& symbol); // .extern
    void addGlobal(const string& symbol); // .global
    void addLabel(const string& symbol); // label:

    void addEqu(const string& equSymbol, int value, const vector<pair<int, string>>& usedSymbols); // .equ

    void resolveSymbols();
    void backpatch();

    void generateObjectFile(const string& path);
};

#endif
