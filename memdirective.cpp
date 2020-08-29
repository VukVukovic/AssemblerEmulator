#include "memdirective.h"

Encoding SkipDirective::getEncoding() const {
  return Encoding(vector<char>(size, 0));
}

void ByteWordDirective::addLiteral(int value) {
  vector<char> bytes;
  Encoding::addBytes(value, size, bytes);
  Encoding newDirective(bytes);
  encoding.add(newDirective);
}

void ByteWordDirective::addSymbol(string symbol) {
  Encoding newDirective(vector<char>(size, 0), SymbolEncodingEntry(symbol, 0, size));
  encoding.add(newDirective);
}
