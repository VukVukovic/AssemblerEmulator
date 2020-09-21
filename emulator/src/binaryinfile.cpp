#include "binaryinfile.h"
#include "emulatorexception.h"
#include <string>

BinaryInFile::BinaryInFile(const string &path) : file(path) {
  infile.open(path, ios::in | ios::binary);
  if (!infile.is_open())
    throw EmulatorException("File " + path + " cannot be opened for input");
}

SymbolEntry BinaryInFile::readSymbolEntry() {
  return SymbolEntry(readString(), readInt(), readString(),
                     (SymbolType)readInt());
}

RelEntry BinaryInFile::readRelEntry() {
  return RelEntry((RelocationType)readInt(), readString(), readInt());
}

ChunkHeader BinaryInFile::readChunkHeader() {
  ChunkHeader chunk;
  infile.read((char *)&chunk, sizeof(chunk));
  return chunk;
}

string BinaryInFile::readString() {
  int size = readInt();
  string result(size, ' ');
  infile.read(&result[0], size);
  return result;
}

int BinaryInFile::readInt() {
  int value;
  infile.read((char *)&value, sizeof(value));
  return value;
}

char BinaryInFile::readChar() {
  char value;
  infile.read((char *)&value, sizeof(value));
  return value;
}
