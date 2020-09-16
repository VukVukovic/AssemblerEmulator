#include "operand.h"
#include "memory.h"

int16_t RegisterDirect::read() const {
  int16_t value = registers[reg];
  if (size == 1 && !high) value &= 0xFF;
  if (size == 1 && high) value >>= 8;
  return value;
}

int16_t RegisterIndirect::read() const {
  uint16_t location = (uint16_t)registers[reg] + displacement;
  int16_t value;
  if (size == 1) value = memory.readByte(location);
  else value = memory.readWord(location);
  return value;
}

int16_t MemLocation::read() const {
  int16_t value;
  if (size == 1) value = memory.readByte(location);
  else value = memory.readWord(location);
  return value;
}

int16_t Immed::read() const {
  return value;
}

void RegisterDirect::write(int16_t value) {
  if (size == 2) registers[reg] = (uint16_t)value;
  else if (high) { registers[reg] &= 0x00FF; registers[reg] |= ((uint16_t)value) << 8; }
  else { registers[reg] &= 0xFF00; registers[reg] |= ((uint16_t)value)&0xFF; }
}

void RegisterIndirect::write(int16_t value) {
  uint16_t location = (uint16_t)registers[reg] + displacement;
  if (size == 1) memory.writeByte(location, value);
  else memory.writeWord(location, value);
}

void MemLocation::write(int16_t value) {
  if (size == 1) memory.writeByte(location, value);
  else memory.writeWord(location, value);
}

bool Immed::isValid(OpCodes operation, bool isDst) const {
  if (operation == TEST || operation == CMP)
    return true;
  if (operation == POP || operation == XCHG)
    return false;
  return !isDst;
}
