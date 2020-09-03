#include <iostream>
#include "instruction.h"
#include "addressing.h"
#include "assemblerexception.h"

const map<string, int> Instruction::opCodes = {
    {"halt", 0}, {"iret", 1}, {"ret", 2}, {"int", 3}, {"call", 4},
    {"jmp", 5}, {"jeq", 6}, {"jne", 7}, {"jgt", 8}, {"push", 9},
    {"pop", 10}, {"xchg", 11}, {"mov", 12}, {"add", 13}, {"sub", 14},
    {"mul", 15}, {"div", 16}, {"cmp", 17}, {"not", 18}, {"and", 19},
    {"or", 20}, {"xor", 21}, {"test", 22}, {"shl", 23}, {"shr", 24}
};

Instruction::Instruction(const string& _type) : type(_type) {
    if (type.back() == 'b') {
        preferedSize = 1;
        type.pop_back();
    } else if (type.back() == 'w') {
        preferedSize = 2;
        type.pop_back();
    }

    if (type == "push" || type == "pop")
        preferedSize = 2;
}

void Instruction::addOperand(Addressing* operand) {
    operands.push_back(operand);
}

bool Instruction::validAdressing() const {
    if (numOfOperands() == 2) {
        if (type == "cmp" || type == "test")
            return true;
        if (type == "xchg" && (operands[0]->getType() == IMMED || operands[1]->getType() == IMMED))
            return false;
        if (operands[1]->getType() == IMMED)
            return false;
    }  else if (numOfOperands() == 1 && type == "pop" && operands[0]->getType() == IMMED) {
        return false;
    }
    return true;
}

Encoding Instruction::getEncoding() const {
    if (!validAdressing())
        throw AssemblerException("Adressing used in instruction is not valid");

    int size = preferedSize;
    for (auto operand : operands) {
        int operandSize = operand->getPreferedSize();

        if (size != operandSize && operandSize != 0) {
            if (size == 0)
                size = operandSize;
            else
                throw AssemblerException("Size of instruction or operands sizes are not matched");
        }
    }

    if (size == 0)
        size = 2; // 2 BYTES is default if not specified differently

    int pcRelOffset = 0;
    for (auto operand : operands)
      pcRelOffset += operand->getBytesSize(size);

    Encoding instructionEncoding({instrutionDescr(size)});
    for (auto operand : operands) {
        operand->setPCRelOffset(pcRelOffset);
        pcRelOffset -= operand->getBytesSize(size);

        instructionEncoding.add(operand->getEncoding(size));
    }

    return instructionEncoding;
}

char Instruction::instrutionDescr(int size) const {
    return (opCodes.at(type) << 3) + ((size == 1 ? 0 : 1) << 2);
}

Instruction::~Instruction() {
  for (auto operand : operands)
    delete operand;
}
