#ifndef READER_H
#define READER_H

#include <map>
#include <string>
#include <vector>
using namespace std;
#include "externtypes.h"

class BinaryInFile;
class Memory;

class Reader {
  map<string, vector<char>> sections;
  map<string, vector<RelEntry>> relocations;
  map<string, SymbolEntry> symbols;
  map<string, vector<string>> waiting;

  void readFile(BinaryInFile &file);
  vector<char> readSection(BinaryInFile &file, int size);
  vector<RelEntry> readRelocation(BinaryInFile &file, int reloNum);
  vector<SymbolEntry> readSymbols(BinaryInFile &file, int symbolNum);
  void
  aggregateAndRelocate(vector<SymbolEntry> &fileSymbols,
                       vector<pair<string, vector<char>>> &fileSections,
                       vector<pair<string, vector<RelEntry>>> &fileRelocations);
  int checkLoadingPlaces(const map<string, int> &places);
  void resolveRelocations(const map<string, int> &startingAddress);

  static void addBytes(int value, int size, int offset, vector<char> &bytes);

public:
  void read(const vector<string> &files);
  void resolveSymbols();
  void load(Memory &memory, const map<string, int> &places);
};

#endif
