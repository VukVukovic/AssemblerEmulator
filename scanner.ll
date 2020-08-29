%{ /* -*- C++ -*- */
# include <cerrno>
# include <climits>
# include <cstdlib>
# include <cstring> // strerror
# include <string>
# include "driver.hh"
# include "parser.hh"
%}

%option noyywrap nounput noinput batch debug

%{
  yy::parser::symbol_type make_INTEGER (const std::string &s, const yy::parser::location_type& loc);
  yy::parser::symbol_type make_CHAR (const std::string &s, const yy::parser::location_type& loc);
%}

blank [ \t\r]
symbol [a-zA-Z_][a-zA-Z_0-9]*

instr_0 (halt|iret|ret)
instr_jmp (call|jmp|jeq|jne|jgt)
instr_1 (push|pop|int)
instr_arith (xchg|mov|add|sub|mul|div|cmp|not|and|or|xor|test|shl|shr)(b|w)?

integer (([1-9][0-9]*)|0)
character '[\x00-\x7F]'

register (r[0-7]|sp|psw)
register_lh (r[0-7]|sp|psw)(l|h)
register_pc pc

%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  yy::location& loc = drv.location;
  // Code run each time yylex is called.
  loc.step ();
%}
{blank}+   loc.step ();
\n+        loc.lines (yyleng); loc.step ();
":"         return yy::parser::make_SEMICOLON(loc);
"-"         return yy::parser::make_MINUS(loc);
"+"         return yy::parser::make_PLUS(loc);
","         return yy::parser::make_COMMA(loc);
"$"         return yy::parser::make_DOLLAR(loc);
"%"         return yy::parser::make_PERCENT(loc);
"*"         return yy::parser::make_STAR(loc);
"("         return yy::parser::make_PAR_OPEN(loc);
")"         return yy::parser::make_PAR_CLOSE(loc);
".global"   return yy::parser::make_GLOBAL(loc);
".extern"   return yy::parser::make_EXTERN(loc);
".section"  return yy::parser::make_SECTION(loc);
".end"      return yy::parser::make_END(loc);
".skip"     return yy::parser::make_SKIP(loc);
".equ"      return yy::parser::make_EQU(loc);
".byte"     return yy::parser::make_BYTE(loc);
".word"     return yy::parser::make_WORD(loc);
{integer}   return make_INTEGER(yytext, loc);
{character} return make_CHAR(yytext, loc);
{register_lh}  return yy::parser::make_REGISTER_LH(yytext, loc);
{register}  return yy::parser::make_REGISTER(yytext, loc);
"pc"        return yy::parser::make_PC_REGISTER(loc);
{instr_0}   return yy::parser::make_INSTR_0(yytext, loc);
{instr_jmp} return yy::parser::make_INSTR_JMP(yytext, loc);
{instr_1}    return yy::parser::make_INSTR_1(yytext, loc);
{instr_arith} return yy::parser::make_INSTR_ARITH(yytext, loc);
{symbol}    return yy::parser::make_SYMBOL(yytext, loc);
.          {
             throw yy::parser::syntax_error
               (loc, "invalid character: " + std::string(yytext));
}
<<EOF>>    return yy::parser::make_END(loc);
%%

yy::parser::symbol_type make_INTEGER (const std::string &s, const yy::parser::location_type& loc)
{
  errno = 0;
  long n = strtol (s.c_str(), NULL, 10);
  if (! (INT_MIN <= n && n <= INT_MAX && errno != ERANGE))
    throw yy::parser::syntax_error (loc, "integer is out of range: " + s);
  return yy::parser::make_INTEGER((int) n, loc);
}

yy::parser::symbol_type make_CHAR (const std::string &s, const yy::parser::location_type& loc)
{
  char c = s[1];
  return yy::parser::make_CHAR((int)c, loc);
}

void driver::scan_begin ()
{
  yy_flex_debug = trace_scanning;
  if (file.empty () || file == "-")
    yyin = stdin;
  else if (!(yyin = fopen (file.c_str (), "r")))
    {
      std::cerr << "cannot open " << file << ": " << strerror(errno) << '\n';
      exit (EXIT_FAILURE);
    }
}

void driver::scan_end ()
{
  fclose (yyin);
}
