#ifndef BINARYINFILE_H
#define BINARYINFILE_H

#include <fstream>
#include "externtypes.h"
using namespace std;

class BinaryInFile {
  ifstream infile;

public:
  BinaryInFile(const string& path);

  template<class T>
  T read() {
    T value;
    infile.read((char*)&value, sizeof(T));
    return value;
  }

  template <>
  string read<string>() {
    int size;
    infile.read((char*)&size, sizeof(int));
    string result(size, ' ');
    infile.read(&result[0], size);
    return result;
  }

  template <>
  SymbolEntry read<SymbolEntry>() {
    return SymbolEntry(read<string>(), read<int>(), read<string>(), read<SymbolType>());
  }

  template <>
  RelEntry read<RelEntry>() {
    return RelEntry(read<RelocationType>(), read<string>(), read<int>());
  }

  bool eof() { return infile.eof(); }

  ~BinaryInFile() { infile.close(); }
};

#endif
