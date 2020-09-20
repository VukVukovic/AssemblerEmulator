#ifndef TIMER_H
#define TIMER_H

class CPU;
class Memory;

class Timer {
  CPU &cpu;
  Memory &memory;

  long long int previousTime;
  long long int duration;
  bool sleeping = false;

public:
  Timer(CPU &cpu, Memory &memory) : cpu(cpu), memory(memory) {}
  void timerTick();
};

#endif
