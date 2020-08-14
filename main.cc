#include <iostream>
#include "driver.hh"
#include "code.h"

int main (int argc, char *argv[])
{
  int res = 0;
  Code code;
  driver drv(code);
  

  // drv.trace_parsing = true;
  // drv.trace_scanning = true;

  if (!drv.parse (argv[1])) {
    for (auto &p : code.encoded) {
      cout << endl << "SECTION: " << p.first << endl;
      for (auto byte : p.second)
        cout << std::hex << byte  << " ";
    }
  }
  
  std::cout << "FINISHED" << std::endl;
  return 0;
}