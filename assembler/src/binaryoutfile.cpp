#include "binaryoutfile.h"
#include "assemblerexception.h"
#include <string>

BinaryOutFile::BinaryOutFile(const string &path) {
  outfile.open(path, ios::out | ios::binary);
  if (!outfile.is_open())
    throw AssemblerException("File " + path + " cannot be opened for output");
}

void BinaryOutFile::write(const string &value) {
  int size = value.length();
  outfile.write((char *)&size, sizeof(size));
  outfile.write(value.c_str(), size);
}

void BinaryOutFile::write(const SymbolEntry &entry) {
  write(entry.symbol);
  write(entry.value);
  write(entry.reference);
  write(entry.type);
}

void BinaryOutFile::write(const RelEntry &entry) {
  write(entry.type);
  write(entry.symbol);
  write(entry.offset);
}
