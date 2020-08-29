#include <iostream>
#include <iomanip>
#include "driver.hh"
#include "code.h"

char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

string getHex(char byte) {
  string s;
  s += hex_chars[ ( byte & 0xF0 ) >> 4 ];
  s += hex_chars[ ( byte & 0x0F ) >> 0 ];
  return s;
}

int main (int argc, char *argv[])
{
  int res = 0;
  Code code;
  driver drv(code);


  // drv.trace_parsing = true;
  // drv.trace_scanning = true;

  if (!drv.parse (argv[1])) {
    for (auto &p : code.encodedSections) {
      cout << endl << "SECTION: " << p.first << endl;
      for (auto byte : p.second.getBytes())
        cout << getHex(byte)  << " ";
    }
  }

  try {
    code.symbolTable.resolveSymbols();
    cout << endl << "---------------------" << endl;
    cout << "SYMBOLS: "<< endl;
    code.symbolTable.printSymbolTable(cout);
  } catch (const AssemblerException& e) {
    cout << e.what() << " (" << e.getProblem() << ")" << endl;
  }

  std::cout << "FINISHED" << std::endl;
  return 0;
}
