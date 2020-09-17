#ifndef CPU_H
#define CPU_H
#include <cstdint>
#include <vector>
#include <mutex>
#include "typesconstants.h"
#include <chrono>

using namespace std;
using namespace std::chrono;

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
  mutex interruptsMutex;

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

  struct Timer {
    milliseconds previousTime;
    milliseconds duration;
    bool sleeping = false;
  };
  Timer timer;
  void timerTick();

public:
  CPU(Memory& memory) : memory(memory), registers(REG_NUM, 0) {};
  void start();
  void interruptMark(int i);
};

#endif
