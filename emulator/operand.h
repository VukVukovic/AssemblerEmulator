#ifndef OPERAND_H
#define OPERAND_H

class Operand {
protected:
  int size;

};

class Register : public Operand {

};

class MemLocation : public Operand {
  int location;

  
};

class Immed : public Operand {

};
#endif
