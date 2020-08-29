#ifndef SYMBOLLITERALLIST_H
#define SYMBOLLITERALLIST_H

class SymLitNode {
public:
  virtual char type() = 0;
  virtual ~SymLitNode() {}
};

struct SymLitSignNode {
  int sign;
  SymLitNode *node;
};

class SymbolNode : public SymLitNode {
public:
  string symbol;
  SymbolNode(string symbol) : symbol(symbol) {};
  virtual char type() { return 'S'; };
};

class LiteralNode : public SymLitNode {
public:
  int literal;
  LiteralNode(int literal) : literal(literal) {};
  virtual char type() { return 'L'; };
};

#endif
