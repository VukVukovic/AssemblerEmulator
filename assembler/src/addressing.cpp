#include "addressing.h"

Encoding ImmedLiteral::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(value, size, bytes); // 1 or 2 bytes
  return Encoding(bytes);
}

Encoding ImmedSymbol::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(0, size, bytes); // 1 or 2 bytes
  return Encoding(bytes, SymbolEncodingEntry(symbol, 1, size));
}

Encoding RegisterDirect::getEncoding(int size) const {
  return Encoding({operandDescr()});
}

Encoding RegisterIndirect::getEncoding(int size) const {
  return Encoding({operandDescr()});
}

Encoding RegisterIndDispLiteral::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(value, 2, bytes); // displacement is always 2 bytes
  return Encoding(bytes);
}

Encoding RegisterIndDispSymbol::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(0, 2, bytes); // displacement is always 2 bytes
  return Encoding(bytes, SymbolEncodingEntry(symbol, 1, 2));
}

Encoding PCRelative::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(0, 2, bytes);
  return Encoding(bytes, SymbolEncodingEntry(symbol, 1, 2, pcRelOffset));
}

Encoding MemDirLiteral::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(value, 2, bytes); // Address in memory is always 2 bytes
  return Encoding(bytes);
}

Encoding MemDirSymbol::getEncoding(int size) const {
  vector<char> bytes = {operandDescr()};
  Encoding::appendBytes(0, 2, bytes); // Address in memory is always 2 bytes
  return Encoding(bytes, SymbolEncodingEntry(symbol, 1, 2));
}
