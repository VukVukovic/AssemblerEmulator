#ifndef DRIVER_H
# define DRIVER_H
# include <string>
# include <map>
# include "parser.h"

# define YY_DECL \
  yy::parser::symbol_type yylex (driver& drv)
YY_DECL;

class driver
{
  Code& code;
public:
  driver (Code &code);

  int result;
  int parse (const std::string& f);
  std::string file;
  bool trace_parsing;
  void scan_begin ();
  void scan_end ();
  bool trace_scanning;
  yy::location location;

  Code& getCode() { return code; }
};
#endif
