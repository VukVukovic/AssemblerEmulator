#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <vector>
#include "typesconstants.h"
#include "timer.h"
#include "terminal.h"

using namespace std;

class Memory;
class Operand;

struct InvalidInstruction : public exception {
    virtual const char* what() const throw() {
        return "Invalid instruction";
    }
};

class CPU {
  vector<int16_t> registers;
  uint16_t& pc = (uint16_t&)registers[PC];
  uint16_t& sp = (uint16_t&)registers[SP];
  uint16_t& psw = (uint16_t&)registers[PSW];

  enum PswBit { Z, O, C, N, I};
  static const uint16_t MASKS[];
  static const vector<int> operandNum;

  Memory& memory;

  struct Instruction {
    OpCodes opCode;
    int size;
    vector<Operand*> operands;
  };

  struct ALUUnit{
    int16_t src;
    int16_t dst;
    int16_t result;
  };

  ALUUnit ALU;

  vector<bool> interrupts;

  Instruction current;
  bool running = false;

  void readInstruction();
  void clearCurrentOperands();
  void executeInstruction();
  void checkInterrupts();
  void goToInterrupt(int i);

  void stackPush(int16_t value);
  int16_t stackPop();

  bool readPswBit(PswBit bit) const;
  void setPswBit(PswBit bit, bool value);
  bool equal() const;
  bool signedGreater() const;
  void updatePSW();

  void runALU();

  Timer timer;
  Terminal terminal;

public:
  CPU(Memory& memory) : registers(REG_NUM, 0), memory(memory), interrupts(INTERRUPT_ENTRIES, false), timer(*this, memory), terminal(*this, memory) {};
  void start();
  void interruptMark(int i);
  bool canRequest(int i) { return !interrupts[i]; }

  static const int INTERRUPT_ENTRIES = 8;
  static const int INAVLID_INTERRUPT = 1;
  static const int TIMER_INTERRUPT = 2;
  static const int TERMINAL_INTERRUPT = 3;
};

#endif
