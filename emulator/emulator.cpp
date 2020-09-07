#include <iostream>
#include "reader.h"
#include "emulatorexception.h"

using namespace std;

int main() {
  Reader reader;
  vector<string> files;
  files.push_back("../file1.o");
  files.push_back("../file2.o");

  map<string, int> places;
  places["text"] = 200;

  try {
    reader.read(files);
    reader.load(places);
  } catch(const EmulatorException& e) {
    cout << e.getProblem() << endl;
  }

  return 0;
}
