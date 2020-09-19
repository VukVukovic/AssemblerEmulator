#ifndef TERMINAL_H
#define TERMINAL_H
#include <termios.h>
#include <thread>
using namespace std;

struct termios;
class CPU;
class Memory;

class Terminal {
  CPU* cpu = nullptr;
  Memory* memory = nullptr;
  struct termios originalTerm;

  static void reading();
  thread readingThread;
  bool started = false;
  bool ending = false;

private:
  Terminal() {}
  static void cleanUpTerminal();

public:
  void setCPU(CPU* cpu) { this->cpu = cpu; }
  void setMemory(Memory* memory) { this->memory = memory; }

  void start();
  void stop();
  void newData();

  Terminal(Terminal const&) = delete;
  void operator=(Terminal const&) = delete;

  static Terminal& getInstance()
  {
    static Terminal instance;
    return instance;
  }
};
#endif
