#ifndef MEMDIRECTIVE_H
#define MEMDIRECTIVE_H

#include <string>
#include "encoding.h"
using namespace std;

class MemDirective {
public:
  virtual Encoding getEncoding() const = 0;
  virtual ~MemDirective() {}
};

class SkipDirective : public MemDirective {
  int size;
public:
  SkipDirective(int size) : size(size) {}
  virtual Encoding getEncoding() const;
};

class ByteWordDirective : public MemDirective {
  int size;
  Encoding encoding;
public:
  ByteWordDirective(int size) : size(size) {}
  virtual Encoding getEncoding() const { return encoding; }
  void addLiteral(int value);
  void addSymbol(string symbol);
};
#endif
