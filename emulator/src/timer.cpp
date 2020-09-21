#include "timer.h"
#include "cpu.h"
#include "memory.h"
#include "typesconstants.h"
#include <chrono>
#include <iostream>
using namespace std;
using namespace std::chrono;

void Timer::timerTick() {
  long long int current =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch())
          .count();

  if (sleeping && current - previousTime >= duration) {
    sleeping = false;
    cpu.interruptMark(CPU::TIMER_INTERRUPT);
  }

  if (!sleeping) {
    int8_t timerCfg = (memory.read(TIMER_CFG, 2) & 0x7);
    switch (timerCfg) {
    case 0x0:
      duration = 500;
      break;
    case 0x1:
      duration = 1000;
      break;
    case 0x2:
      duration = 1500;
      break;
    case 0x3:
      duration = 2000;
      break;
    case 0x4:
      duration = 5000;
      break;
    case 0x5:
      duration = 10000;
      break;
    case 0x6:
      duration = 30000;
      break;
    case 0x7:
      duration = 60000;
      break;
    default:
      break;
    }
    sleeping = true;
    previousTime = current;
  }
}
