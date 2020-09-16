#ifndef READER_H
#define READER_H

#include "../commontypes.h"
#include <map>
#include <string>
#include <vector>
using namespace std;

class BinaryInFile;
class Memory;

class Reader {
  map<string, vector<char>> sections;
  map<string, vector<RelEntry>> relocations;
  map<string, SymbolEntry> symbols;
  map<string, vector<string>> waiting;

  void readFile(BinaryInFile& file);
  vector<char> readSection(BinaryInFile& file, int size);
  vector<RelEntry> readRelocation(BinaryInFile& file, int reloNum);
  vector<SymbolEntry> readSymbols(BinaryInFile& file, int symbolNum);
  int checkLoadingPlaces(const map<string, int>& places);
  void resolveRelocations(const map<string, int>& startingAddress);
  void resolveSymbol(const string& symbol);
  bool canResolve(const string& symbol);
  bool isResolved(const string& symbol);

public:
  void read(const vector<string>& files);
  void resolveSymbols();
  void load(Memory& memory, const map<string, int>& places);
};

#endif
