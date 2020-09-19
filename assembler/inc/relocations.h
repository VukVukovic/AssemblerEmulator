#ifndef RELOCATIONS_H
#define RELOCATIONS_H

#include <string>
#include <map>
#include <vector>

using namespace std;

enum RelocationType { R_8, R_16, R_PC};

struct RelEntry {
  RelocationType type;
  string symbol;
  int offset;
  RelEntry() = default;
  RelEntry(RelocationType type, const string& symbol, int offset) : type(type), symbol(symbol), offset(offset) {}
};

class Relocations {
  static const string StringRelocationTypes[];
  map<string, vector<RelEntry>> relocations;

public:
  void addRelocation(const string& section, RelocationType type, const string& symbol, int offset);
  bool hasRelocations(const string& section) const { return relocations.find(section) != relocations.end(); }
  void printRelocaions(const string& section, ostream& out, const string& line) const;

  map<string, vector<RelEntry>>::const_iterator begin() const { return relocations.begin(); }
  map<string, vector<RelEntry>>::const_iterator end() const { return relocations.end(); }
};

#endif
