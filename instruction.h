#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include <map>
#include <vector>
#include <string>
#include "encoding.h"
using namespace std;

class Addressing;

struct IncompatibleSizes : public exception {
    const char* what() const throw() {
        return "Size of instruction or operands sizes are not matched";
    }
};

struct BadAdressing : public exception {
    const char* what() const throw() {
        return "Adressing used in instruction is not valid";
    }
};

class Instruction {
    string type;
    int preferedSize = 0;
    vector<Addressing*> operands;
    char instrutionDescr(int size) const;
    bool validAdressing() const;

protected:
    static const map<string, int> opCodes;

public:
    Instruction(const string& type);
    void addOperand(Addressing* operand);
    int numOfOperands() const { return operands.size(); }
    Encoding getEncoding() const;
};
#endif