#ifndef BINARYINFILE_H
#define BINARYINFILE_H

#include <fstream>
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

  bool eof() { return infile.eof(); }

  ~BinaryInFile() { infile.close(); }
};

#endif
