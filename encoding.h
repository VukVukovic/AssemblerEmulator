#ifndef ENCODING_H
#define ENCODING_H
#include <vector>
#include <string>
using namespace std;

struct SymbolEncodingEntry {
    string symbol;
    int location;
    int size;
    SymbolEncodingEntry(const string& symbol, int location, int size) : symbol(symbol), location(location), size(size) {}
};

class Encoding {
  int offset = 0;
  vector<char> bytes;
  vector<SymbolEncodingEntry> symbolLocations;

public:
  Encoding() = default;
  Encoding(const vector<char>& bytes) : bytes(bytes) {}
  Encoding(const vector<char>& bytes, const SymbolEncodingEntry& see) : bytes(bytes), symbolLocations({see}) {}

  static void addBytes(int value, int size, vector<char>& bytes);
  void add(const Encoding& encoding);
  const vector<char>& getBytes() const { return bytes; }
  const vector<SymbolEncodingEntry>& getSymbolLocations() const { return symbolLocations; }
};
#endif
