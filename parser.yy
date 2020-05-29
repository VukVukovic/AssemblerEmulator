%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.5"
%defines

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  # include <string>
  class driver;
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose

%code {
#include "driver.hh"
#include <iostream>

using namespace std;

}

%define api.token.prefix {TOK_}
%token
  END  0  "end of file"
  SEMICOLON ":"
  PLUS "+"
  MINUS "-"
  COMMA ","
  GLOBAL
  EXTERN
  SECTION
  END_DIR
  SKIP
  EQU
  BYTE
  WORD
;

%token <std::string> SYMBOL
%token <std::string> LITERAL
%token <std::string> INSTR_ARITH

%printer { yyo << $$; } <*>;

%%
%start program;
program: expressions  {};

expressions:
	%empty                 {}
|	expressions expression {};

expression:
	directive 	{}
|	label 		  {}
|	instruction	{};

label:
	SYMBOL ":" {};

directive:
	GLOBAL symbol_list			{}
|	EXTERN symbol_list			{}
|	SECTION SYMBOL ":"			{ std::cout <<  "SECTION: " << $2 << std::endl; }
|	END_DIR							{}
|	SKIP LITERAL				{}
|	EQU SYMBOL "," sym_lit_expr	{}
|	BYTE sym_lit_list			{}
|	WORD sym_lit_list			{};


symbol_list:
	SYMBOL 					{}
|	symbol_list "," SYMBOL  {};

sym_lit:
	SYMBOL {}
|	LITERAL{};

sym_lit_list:
	sym_lit 				{}
|	sym_lit_list "," sym_lit{};

sym_lit_expr:
  sym_lit  {}
| sym_lit sym_lit_expr_rest {}
| sym_lit_expr_rest {};

sym_lit_expr_rest:
  "+" sym_lit {}
| "-" sym_lit {}
|	"+" sym_lit sym_lit_expr_rest	{}
| "-"	sym_lit sym_lit_expr_rest {};

instruction:
	INSTR_ARITH operand "," operand     {};

operand:
	SYMBOL {};
%%

void
yy::parser::error (const location_type& l, const std::string& m)
{
  std::cerr << l << ": " << m << '\n';
}