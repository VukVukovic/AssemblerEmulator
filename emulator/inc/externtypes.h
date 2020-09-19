#ifndef EXTERNTYPES_H
#define EXTERNTYPES_H

#include <string>
using namespace std;

enum ChunkType { SECTION, RELOCATION, SYMBOLS };

struct ChunkHeader {
  ChunkType type;
  int size;
};

enum SymbolType { EXT, ABS, REL };

struct SymbolEntry {
  string symbol;
  int value = 0;
  string reference;
  SymbolType type;
  SymbolEntry() = default;
  SymbolEntry(const string& symbol, int value, const string& reference, SymbolType type) :
    symbol(symbol), value(value), reference(reference), type(type) {}
};

enum RelocationType { R_8, R_16, R_PC};

struct RelEntry {
  RelocationType type;
  string symbol;
  int offset;
  RelEntry() = default;
  RelEntry(RelocationType type, const string& symbol, int offset) : type(type), symbol(symbol), offset(offset) {}
};

#endif
