#ifndef RELOCATIONS_H
#define RELOCATIONS_H

#include <string>
#include <map>
#include <vector>
#include "commontypes.h"

using namespace std;

class Relocations {
  map<string, vector<RelEntry>> relocations;

public:
  void addRelocation(const string& section, RelocationType type, const string& symbol, int offset);

  map<string, vector<RelEntry>>::const_iterator begin() const { return relocations.begin(); }
  map<string, vector<RelEntry>>::const_iterator end() const { return relocations.end(); }
};

#endif
