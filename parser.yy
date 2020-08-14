%skeleton "lalr1.cc" /* -*- C++ -*- */
%require "3.6.3"
%defines

%define api.token.constructor
%define api.value.type variant
//%define parse.assert

%code requires {
  #include <string>
  class driver;
  class Code;
  #include <vector>
  #include "register.h"
  #include "addressing.h"
  #include "instruction.h"
}

// The parsing context.
%param { driver& drv }

%locations

%define parse.trace
%define parse.error verbose 

%code {
  #include "driver.hh"
  #include "code.h"
  #include <iostream>
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
  DOLLAR "$"
  PERCENT "%"
  STAR "*"
  PAR_OPEN "("
  PAR_CLOSE ")"
  PC_REGISTER
;

%token <std::string> REGISTER
%token <std::string> REGISTER_LH
%token <std::string> SYMBOL
%token <int> INTEGER
%token <int> CHAR

// string za sada
%token <std::string> INSTR_ARITH
%token <std::string> INSTR_0
%token <std::string> INSTR_JMP
%token <std::string> INSTR_1

%type <Addressing*> operand;
%type <Addressing*> jump_operand;
%type <Register> register;
%type <Register> register_lh;
%type <int> literal;
%type <int> literal_no_sign;
%type <int> literal_sign;
%type <std::vector<std::string>*> symbol_list;
%type <Instruction*> instruction;

%printer { yyo << $$; } <*>;

%%
%start _program;
_program: 
  program  { std::cout << "ALL" << std::endl; };

program:
  preamble sections {};

preamble:
  %empty {}
| preamble indep_directive {};

indep_directive:
  GLOBAL symbol_list			{
    for (auto s : *($2)) 
      cout << "sym:" << s << endl;
   }
|	EXTERN symbol_list			{}
|	EQU SYMBOL "," sym_lit_expr	{};

symbol_list:
	SYMBOL 					{ $$ = new std::vector<std::string>({$1}); }
|	symbol_list "," SYMBOL  { ($1)->push_back($3); $$ = $1; };

sym_lit_list:
	SYMBOL  { }
| literal { }
|	sym_lit_list "," SYMBOL {  }
|	sym_lit_list "," literal { };

literal:
  literal_sign { $$ = $1; }
| literal_no_sign { $$ = $1; };

literal_no_sign:
  INTEGER { $$ = $1; }
| CHAR { $$ = $1; };

literal_sign:
  "+" literal_no_sign { $$ = $2; }
| "-" literal_no_sign { $$ = -$2; };

sym_lit:
  SYMBOL { }
| literal_no_sign { };

sym_lit_expr:
  "+" sym_lit expr_rest {}
| "-" sym_lit expr_rest {}
| sym_lit expr_rest {};

expr_rest:
  %empty {}
| "+" sym_lit expr_rest {}
| "-" sym_lit expr_rest {};

sections:
  %empty {}
| section sections {};

section:
  SECTION SYMBOL ":" { drv.getCode().beginSection($2); } expressions { };

expressions:
	%empty { }
|	expressions expression { };

expression:
	directive 	{}
|	label 		  {}
|	instruction	{ try { drv.getCode().addInstruction($1->getEncoding()); } catch(exception& e) { throw yy::parser::syntax_error(drv.location, e.what()); } };

directive:
	indep_directive {}
|	END_DIR { /* yy acept */}
|	SKIP literal {}
|	BYTE sym_lit_list {}
|	WORD sym_lit_list {};

label:
	SYMBOL ":" {};

instruction:
  INSTR_0 { $$ = new Instruction($1); }
| INSTR_JMP jump_operand { $$ = new Instruction($1); $$->addOperand($2); }
| INSTR_1 operand { $$ = new Instruction($1); $$->addOperand($2); }
| INSTR_ARITH operand "," operand { $$ = new Instruction($1); $$->addOperand($2); $$->addOperand($4); };

operand:
  "$" literal { $$ = new ImmedLiteral($2); }
| "$" SYMBOL { $$ = new ImmedSymbol($2); }
| register_lh { $$ = new RegisterDirect($1); }
| register { $$ = new RegisterDirect($1); }
| "(" register ")" { $$ = new RegisterIndirect($2); }
| literal "(" register ")" { $$ = new RegisterIndDispLiteral($3, $1); }
| SYMBOL "(" register ")" { $$ = new RegisterIndDispSymbol($3, $1); }
| SYMBOL "(" pc_register ")" { $$ = new PCRelative($1); }
| literal { $$ = new MemDirLiteral($1); }
| SYMBOL { $$ = new MemDirSymbol($1); };

jump_operand:
  literal { $$ = new ImmedLiteral($1); }
| SYMBOL { $$ = new ImmedSymbol($1); }
| "*" register { $$ = new RegisterDirect($2); }
| "*" "(" register ")" { $$ = new RegisterIndirect($3); }
| "*" literal "(" register ")" { $$ = new RegisterIndDispLiteral($4, $2); }
| "*" SYMBOL "(" register ")" { $$ = new RegisterIndDispSymbol($4, $2); }
| "*" SYMBOL "(" pc_register ")" { $$ = new PCRelative($2); }
| "*" literal { $$ = new MemDirLiteral($2); }
| "*" SYMBOL { $$ = new MemDirSymbol($2); };

register:
  "%" REGISTER { $$ = Register($2); };

register_lh:
  "%" REGISTER_LH { $$ = Register($2); };

pc_register:
  "%" PC_REGISTER {};
%%

void yy::parser::error (const location_type& l, const std::string& m) {
  std::cerr << l << ": " << m << '\n';
}