#ifndef ENCODING_H
#define ENCODING_H
#include <string>
#include <vector>
using namespace std;

struct SymbolEncodingEntry {
  string symbol;
  int location;
  int size;
  int pcRelOffset;
  SymbolEncodingEntry(const string &symbol, int location, int size,
                      int pcRelOffset)
      : symbol(symbol), location(location), size(size),
        pcRelOffset(pcRelOffset) {}
  SymbolEncodingEntry(const string &symbol, int location, int size)
      : SymbolEncodingEntry(symbol, location, size, 0) {}
};

class Encoding {
  // int offset = 0;
  vector<char> bytes;
  vector<SymbolEncodingEntry> symbolLocations;

public:
  Encoding() = default;
  Encoding(const vector<char> &bytes) : bytes(bytes) {}
  Encoding(const vector<char> &bytes, const SymbolEncodingEntry &see)
      : bytes(bytes), symbolLocations({see}) {}

  static void appendBytes(int value, int size, vector<char> &bytes);
  static void addBytes(int value, int size, int offset, vector<char> &bytes);

  void add(const Encoding &encoding);
  const vector<char> &getBytes() const { return bytes; }
  const vector<SymbolEncodingEntry> &getSymbolLocations() const {
    return symbolLocations;
  }

  friend class Code;
};
#endif
