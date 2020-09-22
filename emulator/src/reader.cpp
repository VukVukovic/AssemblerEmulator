#include "reader.h"
#include "binaryinfile.h"
#include "emulatorexception.h"
#include "memory.h"
#include <iostream>
/*
string relos[] = {"R_8", "R_16", "R_PC"};
string types[] = {"EXT", "ABS", "REL"};
char const hex_chars[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                            '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

string getHex(char byte) {
  string s;
  s += hex_chars[(byte & 0xF0) >> 4];
  s += hex_chars[(byte & 0x0F) >> 0];
  return s;
}*/

void Reader::read(const vector<string> &files) {
  for (const string &path : files) {
    BinaryInFile file(path);
    readFile(file);
  }

  //cout << "---------------------" << endl << "SYMBOLS" << endl;
  for (const auto &se : symbols) {
    //cout << se.second.symbol << '\t' << se.second.value << '\t'
      //   << se.second.reference << '\t' << types[se.second.type] << endl;
    if (sections.find(se.first) != sections.end())
      throw EmulatorException("Duplicate symbol " + se.first + " as section");
  }

/*
  cout << "---------------------" << endl << "RELOCATIONS";
  for (const auto &reloSection : relocations) {
    cout << "SECTION " << reloSection.first << endl;
    for (const auto &relo : reloSection.second)
      cout << relo.symbol << '\t' << relo.offset << '\t' << relos[relo.type]
           << endl;
  }

  cout << "---------------------" << endl << "SECTIONS" << endl;
  for (const auto &section : sections) {
    cout << "SECTION " << section.first << endl;
    for (const auto &byte : section.second)
      cout << getHex(byte) << " ";
    cout << endl;
  } */
}

void Reader::readFile(BinaryInFile &file) {
  vector<SymbolEntry> fileSymbols;
  vector<pair<string, vector<char>>> fileSections;
  vector<pair<string, vector<RelEntry>>> fileRelocations;

  try {
    while (!file.eof()) {
      ChunkHeader header = file.readChunkHeader();

      if (header.type == SYMBOLS) {
        fileSymbols = readSymbols(file, header.size);
      } else if (header.type == SECTION) {
        string section = file.readString();
        fileSections.push_back({section, readSection(file, header.size)});
      } else if (header.type == RELOCATION) {
        string section = file.readString();
        fileRelocations.push_back({section, readRelocation(file, header.size)});
      }
    }
  } catch (exception &e) {
    throw EmulatorException("Error while reading file " + file.getName() +
                            ": " + e.what());
  }

  aggregateAndRelocate(fileSymbols, fileSections, fileRelocations);
}

vector<char> Reader::readSection(BinaryInFile &file, int size) {
  vector<char> currentBytes;
  for (int i = 0; i < size; i++)
    currentBytes.push_back(file.readChar());
  return currentBytes;
}

vector<RelEntry> Reader::readRelocation(BinaryInFile &file, int reloNum) {
  vector<RelEntry> relocations;
  for (int i = 0; i < reloNum; i++)
    relocations.push_back(file.readRelEntry());
  return relocations;
}

vector<SymbolEntry> Reader::readSymbols(BinaryInFile &file, int symbolNum) {
  vector<SymbolEntry> symbols;
  for (int i = 0; i < symbolNum; i++)
    symbols.push_back(file.readSymbolEntry());
  return symbols;
}

void Reader::aggregateAndRelocate(
    vector<SymbolEntry> &fileSymbols,
    vector<pair<string, vector<char>>> &fileSections,
    vector<pair<string, vector<RelEntry>>> &fileRelocations) {

  map<string, int> offsets;

  // Get offsets of current aggregate sections
  for (const auto &section : fileSections)
    offsets[section.first] = sections[section.first].size();

  for (auto &se : fileSymbols) {
    // update symbol values that are dependant of sections
    if (se.type == REL && offsets.find(se.reference) != offsets.end()) {
      se.value += offsets[se.reference];
    }

    if (symbols.find(se.symbol) != symbols.end())
      throw EmulatorException("Duplicate symbol " + se.symbol);

    if (se.type == EXT)
      throw EmulatorException("EXT symbol found in exported symbols " +
                              se.symbol);

    symbols[se.symbol] = se;
  }

  // add section bytes to the end of aggregate sections
  for (const auto &section : fileSections)
    sections[section.first].insert(sections[section.first].end(),
                                   section.second.begin(),
                                   section.second.end());

  for (auto &fr : fileRelocations) {
    const string &reloForSection = fr.first;
    for (auto &relo : fr.second) {
      // change offset for relocation changing in memory
      if (offsets.find(reloForSection) != offsets.end())
        relo.offset += offsets[reloForSection];

      if (offsets.find(relo.symbol) != offsets.end()) {
        // change offset in bytes for symbol that references section
        addBytes(offsets[relo.symbol], relo.type == R_8 ? 1 : 2, relo.offset,
                 sections[reloForSection]);
      }
      relocations[reloForSection].push_back(relo);
    }
  }
}

int Reader::checkLoadingPlaces(const map<string, int> &places) {
  vector<pair<int, int>> taken;
  // taken.push_back({0xFF00, 256}); // no check for memory mapped registers

  for (const auto &place : places) {
    if (sections.find(place.first) == sections.end())
      throw EmulatorException("Section " + place.first +
                              " does not exist and cannot be placed");

    int start = place.second;
    int sectionSize = sections[place.first].size();

    //cout << place.first << " " << start << " " << sectionSize;
    if (start < 0 || start + sectionSize > MAPPED_REG_START)
      throw EmulatorException("Place for " + place.first +
                              " is invalid (memory overflow)");

    for (const auto &t : taken) {
      if (!(start + sectionSize <= t.first || start >= t.first + t.second))
        throw EmulatorException(
            "Section " + place.first +
            " cannot be placed at desired location because of overlapping");
    }

    taken.push_back({start, sectionSize});
  }

  int maxTaken = 0;
  for (const auto &t : taken)
    maxTaken = max(maxTaken, t.first + t.second);

  return maxTaken;
}

void Reader::resolveRelocations(const map<string, int> &startingAddress) {
  string notResolved;
  bool error = false;

  for (const auto &reloSection : relocations) {
    const string &reloForSection = reloSection.first;
    for (const auto &relo : reloSection.second) {
      if (relo.type == R_PC) {
        addBytes(-relo.offset, 2, relo.offset, sections[reloForSection]);
        addBytes(-startingAddress.at(reloForSection), 2, relo.offset,
                 sections[reloForSection]);
      }

      if (sections.find(relo.symbol) != sections.end()) {
        // relocation for section
        addBytes(startingAddress.at(relo.symbol), relo.type == R_8 ? 1 : 2,
                 relo.offset, sections[reloForSection]);
      } else if (symbols.find(relo.symbol) != symbols.end()) {
        // reloction for other symbols
        addBytes(symbols[relo.symbol].value, relo.type == R_8 ? 1 : 2,
                 relo.offset, sections[reloForSection]);
        if (symbols[relo.symbol].type == REL)
          addBytes(startingAddress.at(symbols[relo.symbol].reference),
                   relo.type == R_8 ? 1 : 2, relo.offset,
                   sections[reloForSection]);
      } else {
        error = true;
        if (notResolved.length() > 0)
          notResolved += ", ";
        notResolved += relo.symbol;
      }
    }
  }

  if (error)
    throw EmulatorException("Following symbols could not have been resolved: " +
                            notResolved);
}

void Reader::load(Memory &memory, const map<string, int> &places) {
  int startingAddressFree = checkLoadingPlaces(places);

  map<string, int> startingAddress(places);

  for (const auto &section : sections) {
    if (startingAddress.find(section.first) != startingAddress.end())
      continue;

    int start = startingAddressFree;
    int size = section.second.size();

    if (start + size > MAPPED_REG_START) {
      throw EmulatorException("Cannot load all sections into memory");
    }

    startingAddress[section.first] = start;
    startingAddressFree += size;
  }

  resolveRelocations(startingAddress);

//  cout << endl << "SECTIONS AFTER RELOCATIONS" << endl;
  for (const auto &sa : startingAddress) {
//    cout << "SECTION " << sa.first << endl;
//    for (const auto &byte : sections[sa.first])
//      cout << getHex(byte) << " ";
//    cout << endl;
    memory.load(sa.second, sections[sa.first]);
  }
}

void Reader::addBytes(int value, int size, int offset, vector<char> &bytes) {
  int16_t currValue;
  if (size == 1) {
    currValue = bytes[offset];
  } else {
    currValue = *(int16_t *)(&bytes[offset]);
  }
  currValue += value;

  if (size == 1) {
    bytes[offset] = currValue;
  } else {
    *(int16_t *)(&bytes[offset]) = currValue;
  }
}
