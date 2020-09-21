#ifndef OPERAND_H
#define OPERAND_H
#include "typesconstants.h"
#include <cstdint>
#include <vector>
using namespace std;

class Memory;

class Operand {
protected:
  int size;

public:
  Operand(int size) : size(size) {}
  virtual bool isValid(OpCodes operation, bool isDest) const { return true; };
  virtual int16_t read() const = 0;
  virtual void write(int16_t value) = 0;
  virtual ~Operand() {}
};

class RegisterDirect : public Operand {
  int reg;
  bool high;
  vector<int16_t> &registers;

public:
  RegisterDirect(int size, int reg, bool high, vector<int16_t> &registers)
      : Operand(size), reg(reg), high(high), registers(registers) {}
  int16_t read() const override;
  void write(int16_t value) override;
};

class RegisterIndirect : public Operand {
  int reg;
  int16_t displacement;
  vector<int16_t> &registers;
  Memory &memory;

public:
  RegisterIndirect(int size, int reg, int16_t displacement,
                   vector<int16_t> &registers, Memory &memory)
      : Operand(size), reg(reg), displacement(displacement),
        registers(registers), memory(memory) {}
  int16_t read() const override;
  void write(int16_t value) override;
};

class MemLocation : public Operand {
  uint16_t location;
  Memory &memory;

public:
  MemLocation(int size, uint16_t location, Memory &memory)
      : Operand(size), location(location), memory(memory) {}
  int16_t read() const override;
  void write(int16_t value) override;
};

class Immed : public Operand {
  int16_t value;

public:
  Immed(int size, int16_t value) : Operand(size), value(value) {}
  bool isValid(OpCodes operation, bool isDest) const override;
  int16_t read() const override;
  void write(int16_t value) override {}
};
#endif
