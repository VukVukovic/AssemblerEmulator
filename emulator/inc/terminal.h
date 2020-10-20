#ifndef TERMINAL_H
#define TERMINAL_H
#include <queue>
using namespace std;

class CPU;
class Memory;

class Terminal {
  CPU &cpu;
  Memory &memory;
  queue<char> buffer;
  bool started = false;

public:
  Terminal(CPU &cpu, Memory &memory) : cpu(cpu), memory(memory) {}
  void setup();
  void clean();
  void readInput();
  ~Terminal() {
    if (started)
      clean();
  }
};
#endif
