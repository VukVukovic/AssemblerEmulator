#include <iostream>
#include "reader.h"
#include "emulatorexception.h"
#include "memory.h"
#include "cpu.h"

using namespace std;

int main() {
  Reader reader;
  vector<string> files;
  files.push_back("../file1.o");
  files.push_back("../file2.o");

  map<string, int> places;
  places["text"] = 200;

  Memory memory;
  CPU cpu(memory);
  //Terminal terminal(cpu);

  try {
    reader.read(files);
    reader.resolveSymbols();
    reader.load(memory, places);
    //terminal.start();
    cpu.start();
  } catch(const EmulatorException& e) {
    cout << e.getProblem() << endl;
  }

  //terminal.stop();

  return 0;
}
