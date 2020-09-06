#include "binaryinfile.h"
#include "../assemblerexception.h"
#include <string>

BinaryInFile::BinaryInFile(const string& path) {
  infile.open(path, ios::in | ios::binary);
  if (!infile.is_open())
    throw AssemblerException("File " + path + " cannot be opened for input");
}
