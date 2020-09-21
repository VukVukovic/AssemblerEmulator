#include "code.h"
#include "driver.h"
#include <iostream>
using namespace std;

int main(int argc, char *argv[]) {
  string inputFile, outputFile;
  bool nextOutput = false;
  bool printText = false;
  for (int i = 1; i < argc; i++) {
    string current = argv[i];
    if (nextOutput) {
      outputFile = current;
      nextOutput = false;
    } else if (current == "-t")
      printText = true;
    else if (current == "-o")
      nextOutput = true;
    else
      inputFile = current;
  }

  if (inputFile.size() == 0) {
    cout << "Input file not specified!" << endl;
    return 0;
  }

  Code code;
  driver drv(code);

  if (!drv.parse(inputFile)) {
    try {
      code.resolveSymbols();
      code.backpatch();

      if (printText)
        code.printInfo(cout);

      if (outputFile.size() == 0) {
        if (!printText)
          cout << "Output file not specified!" << endl;
        return 0;
      } else {
        code.generateObjectFile(outputFile);
      }
    } catch (const AssemblerException &e) {
      cout << e.getProblem() << endl;
    }
  }

  return 0;
}
