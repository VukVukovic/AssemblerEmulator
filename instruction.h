#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <map>
#include <set>
#include <vector>
#include <string>
#include "encoding.h"
using namespace std;

class Addressing;

class Instruction {
    string type;
    int preferedSize = 0;
    vector<Addressing*> operands;
    char instrutionDescr(int size) const;
    bool validAdressing() const;

protected:
    static const map<string, int> opCodes;
    static const set<string> wordInstructions;

public:
    Instruction(const string& type);
    void addOperand(Addressing* operand);
    int numOfOperands() const { return operands.size(); }
    Encoding getEncoding() const;
    ~Instruction();
    Instruction(const Instruction&) = delete;
};
#endif
