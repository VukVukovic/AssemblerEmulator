#include "driver.h"
#include "parser.h"

driver::driver(Code &code)
    : code(code), trace_parsing(false), trace_scanning(false) {}

int driver::parse(const std::string &f) {
  file = f;
  location.initialize(&file);
  scan_begin();
  yy::parser parse(*this);
  parse.set_debug_level(trace_parsing);
  int res = parse();
  scan_end();
  return res;
}
