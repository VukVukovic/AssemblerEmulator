#include <iostream>
#include "reader.h"
#include "emulatorexception.h"
#include "memory.h"
#include "cpu.h"
#include <regex>
using namespace std;

int main(int argc, char* argv[]) {
  vector<string> files;
  map<string, int> places;
  regex placeRegex("-place=([a-zA-Z_][a-zA-Z_0-9]*)@(0[xX][0-9a-fA-F]+)");

  for (int i = 1; i < argc; i++) {
    string current = argv[i];

    smatch matches;
    if (regex_match(current, matches, placeRegex)) {
      string section = matches.str(1);
      int address = stoi(matches.str(2), nullptr, 16);
      if (places.find(section) != places.end()) {
        cout << "Place for section " << section << " cannot be defined twice" << endl;
        return 0;
      }
      places[section] = address;
    } else {
      files.push_back(current);
    }
  }

  if (files.size() == 0) {
    cout << "No input files provided" << endl;
    return 0;
  }

  Reader reader;
  Memory memory;
  CPU cpu(memory);

  try {
    reader.read(files);
    reader.load(memory, places);
    cpu.start();
  } catch(const EmulatorException& e) {
    cout << e.getProblem() << endl;
  }

  return 0;
}
