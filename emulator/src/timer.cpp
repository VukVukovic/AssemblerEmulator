#include "timer.h"
#include "cpu.h"
#include "typesconstants.h"
#include "memory.h"

void Timer::timerTick() {
  milliseconds current = duration_cast<milliseconds>(
    system_clock::now().time_since_epoch()
  );

  if (sleeping && current - previousTime >= duration) {
    sleeping = false;
    cpu.interruptMark(CPU::TIMER_INTERRUPT);
  }

  if (!sleeping) {
    int8_t timerCfg = (memory.read(TIMER_CFG, 2) & 0x7);
    switch (timerCfg) {
      case 0x0: duration = milliseconds(500); break;
      case 0x1: duration = milliseconds(1000); break;
      case 0x2: duration = milliseconds(1500); break;
      case 0x3: duration = milliseconds(2000); break;
      case 0x4: duration = milliseconds(5000); break;
      case 0x5: duration = milliseconds(10000); break;
      case 0x6: duration = milliseconds(30000); break;
      case 0x7: duration = milliseconds(60000); break;
      default: break;
    }
    sleeping = true;
  }

  previousTime = current;
}
