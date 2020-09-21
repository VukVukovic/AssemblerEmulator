#ifndef BINARYINFILE_H
#define BINARYINFILE_H

#include "externtypes.h"
#include <fstream>
using namespace std;

class BinaryInFile {
  ifstream infile;
  string file;

public:
  BinaryInFile(const string &path);
  string getName() const { return file; }

  SymbolEntry readSymbolEntry();
  RelEntry readRelEntry();
  ChunkHeader readChunkHeader();
  string readString();
  int readInt();
  char readChar();

  bool eof() { return infile.eof(); }
  ~BinaryInFile() { infile.close(); }
};

#endif
