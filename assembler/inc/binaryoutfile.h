#ifndef BINARYOUTFILE_H
#define BINARYOUTFILE_H

#include <fstream>
#include "symboltable.h"
#include "relocations.h"

using namespace std;

class BinaryOutFile {
  ofstream outfile;

public:
  BinaryOutFile(const string& path);

  template<class T>
  void write(const T& value) {
    outfile.write((char*)&value, sizeof(T));
  }

  template <>
  void write<string>(const string& value) {
    int size = value.length();
    outfile.write((char*)&size, sizeof(size));
    outfile.write(value.c_str(), size);
  }

  template <>
  void write<SymbolEntry>(const SymbolEntry& entry) {
    write(entry.symbol);
    write(entry.value);
    write(entry.reference);
    write(entry.type);
  }

  template <>
  void write<RelEntry>(const RelEntry& entry) {
    write(entry.type);
    write(entry.symbol);
    write(entry.offset);
  }

  ~BinaryOutFile() { outfile.close(); }
};

#endif
