#include <iostream>
#include <iomanip>
#include "driver.hh"
#include "code.h"

int main (int argc, char *argv[])
{
  string inputFile, outputFile;
  bool nextOutput = false;
  bool printText = false;
  for (int i = 1; i < argc; i++) {
    string current = argv[i];
    if (nextOutput) {
      outputFile = current;
      nextOutput = false;
    }
    else if (current == "-t")
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

  if (outputFile.size() == 0) {
    cout << "Output file not specified!" << endl;
    return 0;
  }

  Code code;
  driver drv(code);

  if (!drv.parse(inputFile)) {
    try {
      code.resolveSymbols();
      code.backpatch();
      code.generateObjectFile(outputFile);
    } catch (const AssemblerException& e) {
      cout << e.getProblem() << endl;
    }
  }

  return 0;
}
