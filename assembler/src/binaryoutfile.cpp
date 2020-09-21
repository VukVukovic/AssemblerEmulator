#include "binaryoutfile.h"
#include "assemblerexception.h"
#include <string>

BinaryOutFile::BinaryOutFile(const string &path) {
  outfile.open(path, ios::out | ios::binary);
  if (!outfile.is_open())
    throw AssemblerException("File " + path + " cannot be opened for output");
}
