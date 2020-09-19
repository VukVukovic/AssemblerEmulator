#include "binaryinfile.h"
#include "emulatorexception.h"
#include <string>

BinaryInFile::BinaryInFile(const string& path) {
  infile.open(path, ios::in | ios::binary);
  if (!infile.is_open())
    throw EmulatorException("File " + path + " cannot be opened for input");
}
