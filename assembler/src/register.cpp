#include "register.h"

Register::Register(string reg) {
  if (reg.back() == 'h' || reg.back() == 'l') {
    high = (reg.back() == 'h');
    reg.pop_back();
    size = 1;
  }

  if (reg == "psw") {
    regNum = 0xF;
  } else if (reg == "sp") {
    regNum = 6;
  } else {
    reg.erase(0, 1); // remove r
    regNum = stoi(reg);
  }
}