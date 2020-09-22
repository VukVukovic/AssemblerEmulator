#ifndef BINARYOUTFILE_H
#define BINARYOUTFILE_H

#include "relocations.h"
#include "symboltable.h"
#include <fstream>

using namespace std;

class BinaryOutFile {
  ofstream outfile;

public:
  BinaryOutFile(const string &path);

  template <class T> void write(const T &value) {
    outfile.write((char *)&value, sizeof(T));
  }

  void write(const string &value);
  void write(const SymbolEntry &entry);
  void write(const RelEntry &entry);

  ~BinaryOutFile() { outfile.close(); }
};
#endif
