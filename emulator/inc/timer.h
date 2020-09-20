#ifndef TIMER_H
#define TIMER_H
#include <chrono>
using namespace std;
using namespace std::chrono;

class CPU;
class Memory;

class Timer {
  CPU &cpu;
  Memory &memory;

  milliseconds previousTime;
  milliseconds duration;
  bool sleeping = false;

public:
  Timer(CPU &cpu, Memory &memory) : cpu(cpu), memory(memory) {}
  void timerTick();
};

#endif
