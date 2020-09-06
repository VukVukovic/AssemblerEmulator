#ifndef BINARYOUTFILE_H
#define BINARYOUTFILE_H

#include <fstream>
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

  ~BinaryOutFile() { outfile.close(); }
};

#endif
