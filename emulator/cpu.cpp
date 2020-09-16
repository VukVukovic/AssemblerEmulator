#include "cpu.h"
#include "memory.h"
#include "emulatorexception.h"
#include "operand.h"
#include <iostream>
#include <string>
#include <map>

#define WORD 2
#define BYTE 1

const uint16_t CPU::MASKS[] = {0x0001, 0x0002, 0x0004, 0x0008, 0x8000};
const vector<int> CPU::operandNum = {0, 0, 0, 1, 1,
                                      1, 1, 1, 1, 1,
                                      1, 2, 2, 2, 2,
                                      2, 2, 2, 2, 2,
                                      2, 2, 2, 2, 2};

string toInstr[] = {"HALT", "IRET", "RET", "INT", "CALL",
                    "JMP", "JEQ", "JNE", "JGT", "PUSH",
                    "POP", "XCHG", "MOV", "ADD", "SUB",
                    "MUL", "DIV", "CMP", "NOT", "AND",
                    "OR", "XOR", "TEST", "SHL", "SHR"};

void CPU::readInstruction() {
  unsigned char byte = memory.read(pc, BYTE);
  pc++;

  current.size = (byte >> 2)&1 + 1;

  if ((byte >> 3) >= OP_NUM)
    throw InvalidInstruction{};

  current.opCode = (OpCodes)(byte >> 3);

  cout << toInstr[current.opCode] << " ";

  for (int i = 0; i < operandNum[current.opCode]; i++) {
    byte = memory.read(pc, BYTE);
    pc++;

    if ((byte >> 5) >= ADDR_NUM)
      throw InvalidInstruction{};

    AddressType type = (AddressType)(byte >> 5);
    switch (type) {
      case IMMED: {
        int16_t value = memory.read(pc, current.size);
        pc += current.size;
        current.operands.push_back(new Immed(current.size, value));
        cout << "IMMED "; }
        break;
      case REGDIR: {
        int reg = (byte >> 1)&0xF;
        if (reg == 0xF) reg = PSW;
        if (reg >= REG_NUM)
          throw InvalidInstruction{};
        bool high = byte & 1;
        current.operands.push_back(new RegisterDirect(current.size, reg, high, registers));
        cout << "REGDIR ";
      }
      break;
      case REGIND: {
        int reg = (byte >> 1)&0xF;
        if (reg == 0xF) reg = PSW;
        if (reg >= REG_NUM)
          throw InvalidInstruction{};
        current.operands.push_back(new RegisterIndirect(current.size, reg, 0, registers, memory));
        cout << "REGIND ";
      }
      break;
      case REGINDDISP: {
        int reg = (byte >> 1)&0xF;
        if (reg == 0xF) reg = PSW;
        if (reg >= REG_NUM)
          throw InvalidInstruction{};
        int16_t displacement = memory.read(pc, WORD);
        pc += 2;
        current.operands.push_back(new RegisterIndirect(current.size, reg, displacement, registers, memory));
        cout << "REGINDDISP ";
      }
      break;
      case MEM: {
        int16_t location = memory.read(pc, WORD);
        pc += 2;
        current.operands.push_back(new MemLocation(current.size, location, memory));
        cout << "MEM ";
      }
      break;
      default: break;
    }
  }

  cout << endl;

  bool valid = true;
  for (int i = 0; i < current.operands.size(); i++)
    valid &= current.operands[i]->isValid(current.opCode, current.operands.size()==2 && i==2);

  if (!valid)
    throw InvalidInstruction{};
}

void CPU::executeInstruction() {
  switch (current.opCode) {
    case HALT: running = false; break;
    case IRET: psw = stackPop(); pc = stackPop(); break;
    case RET: pc = stackPop(); break;
    case INT: goToInterrupt((current.operands.front()->read()%8)*2); break;
    case CALL: stackPush(pc); pc = current.operands.front()->read(); break;
    case JMP: pc = current.operands.front()->read(); break;
    case JEQ: if (equal()) pc = current.operands.front()->read(); break;
    case JNE: if (!equal()) pc = current.operands.front()->read(); break;
    case JGT: if (signedGreater()) pc = current.operands.front()->read(); break;
    case PUSH: stackPush(current.operands.front()->read()); break;
    case POP: current.operands.front()->write(stackPop()); break;
    case XCHG: {
      int16_t first = current.operands[0]->read();
      int16_t second = current.operands[1]->read();
      current.operands[0]->write(second);
      current.operands[1]->write(first);
    } break;
    default: {
      ALU.src = current.operands[0]->read();
      ALU.dst = current.operands[1]->read();
      runALU();
      if (current.opCode != TEST && current.opCode != CMP)
        current.operands[1]->write(ALU.result);
    }
  }
}

void CPU::stackPush(int16_t value) {
  sp -= 2;
  memory.write(sp, value, WORD);
}

int16_t CPU::stackPop() {
  int16_t ret = memory.read(sp, WORD);
  sp += 2;
  return ret;
}

void CPU::start() {
  pc = 200;
  sp = (uint16_t)MEM_SIZE;
  running = true;

  while (running) {
    int16_t pc_before = pc;
    try {
      readInstruction();
      executeInstruction();
      clearCurrentOperands();
    } catch (const InvalidInstruction& e) {
      pc = pc_before;
      // mark interrupt
      clearCurrentOperands();
    }
  }
}

void CPU::clearCurrentOperands() {
  for (auto operand : current.operands)
    delete operand;
  current.operands.clear();
}

bool CPU::readPswBit(PswBit bit) const {
  return psw & MASKS[bit];
}

void CPU::setPswBit(PswBit bit, bool value) {
  psw &= ~MASKS[bit];
  if (value) psw |= MASKS[bit];
}

bool CPU::equal() const {
  return readPswBit(Z);
}

bool CPU::signedGreater() const {
  return !(readPswBit(N) != readPswBit(O)) && !readPswBit(Z);
}

void CPU::runALU() {
  switch (current.opCode) {
    case MOV: ALU.result = ALU.src; break;
    case ADD: ALU.result = ALU.src + ALU.dst; break;
    case SUB: ALU.result = ALU.dst - ALU.src; break;
    case MUL: ALU.result = ALU.src * ALU.dst; break;
    case DIV: ALU.result = ALU.dst / ALU.src; break;
    case CMP: ALU.result = ALU.dst - ALU.src; break;
    case NOT: ALU.result = ~ALU.src; break;
    case AND: ALU.result = ALU.src & ALU.dst; break;
    case OR: ALU.result = ALU.src | ALU.dst; break;
    case XOR: ALU.result = ALU.src ^ ALU.dst; break;
    case TEST: ALU.result = ALU.src & ALU.dst; break;
    case SHL: ALU.result = ALU.dst << ALU.src; break;
    case SHR: ALU.result = ALU.dst >> ALU.src; break;
    default: break;
  }

  setPswBit(Z, ALU.result == 0);
  setPswBit(N, ALU.result < 0);

  uint16_t bitMask = (1 << (2*current.size-1)); // takes highest bit

  switch (current.opCode) {
    case ADD: {
      setPswBit(O, ALU.result < ALU.src);
      setPswBit(C, ((ALU.src ^ ALU.dst ^ bitMask) & (ALU.result ^ ALU.src)) & bitMask);
    } break;
    case SUB: case CMP: {
      setPswBit(O, (ALU.src ^ ALU.dst) & (ALU.dst ^ ALU.result) & bitMask);
      setPswBit(C, ALU.dst < ALU.src);
    } break;
    case SHL: {
      setPswBit(C, (ALU.src < 2*current.size) && ((ALU.dst >> (2*current.size-ALU.src)) & 1));
    } break;
    case SHR: {
      setPswBit(C, (ALU.dst >> (ALU.src - 1)) & 1);
    } break;
    default: break;
  }
}

void CPU::checkInterrupts() {
  if (readPswBit(I)) {
    int interruptHappened = -1;
    interruptsMutex.lock();
    for (int i = 0; i < interrupts.size(); i++) {
      if (interrupts[i]) {
        interruptHappened = i;
        break;
      }
    }
    interrupts[interruptHappened] = false;
    interruptsMutex.unlock();

    goToInterrupt(interruptHappened);
  }
}

void CPU::interruptMark(int i) {
  interruptsMutex.lock();
  interrupts[i] = true;
  interruptsMutex.unlock();
}

void CPU::goToInterrupt(int i) {
  stackPush(pc);
  stackPush(psw);
  pc = memory.read(i, WORD);
  setPswBit(I, false);
}
