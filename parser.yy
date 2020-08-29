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
  #include "symbolliterallist.h"
  #include "memdirective.h"
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
%type <std::vector<SymLitNode*>*> sym_lit_list;
%type <std::vector<SymLitSignNode>*> sym_lit_expr;
%type <std::vector<SymLitSignNode>*> expr_rest;
%type <SymLitNode*> sym_lit;
%type <std::string> label;
%type <MemDirective*> mem_directive;

%printer { yyo << $$; } <*>;

%%
%start _program;
_program:
  program  { };

program:
  preamble sections {};

preamble:
  %empty {}
| preamble indep_directive {};

indep_directive:
  GLOBAL symbol_list {
    for (std::string symbol : *($2)) {
      drv.getCode().addGlobal(symbol);
    }
    delete $2;
  }
|	EXTERN symbol_list			{
  for (std::string symbol : *($2)) {
    drv.getCode().addExtern(symbol);
  }
  delete $2;
}
|	EQU SYMBOL "," sym_lit_expr	{
  int value = 0;
  vector<pair<int, string>> symbols;
  for (auto n : *($4)) {
    if (n.node->type() == 'L')
      value += n.sign*((LiteralNode*)n.node)->literal;
    else if (n.node->type() == 'S')
      symbols.push_back({n.sign, ((SymbolNode*)n.node)->symbol});
  }
  try {
    drv.getCode().addEqu($2, value, symbols);
  } catch(exception& e) {
    for (auto n : *($4))
      delete n.node;
    delete $4;
    throw yy::parser::syntax_error(drv.location, e.what());
  }

  for (auto n : *($4))
    delete n.node;
  delete $4;
};

symbol_list:
	SYMBOL 					{ $$ = new std::vector<std::string>({$1}); }
|	symbol_list "," SYMBOL  { ($1)->push_back($3); $$ = $1; };

sym_lit_list:
	SYMBOL  { $$ = new std::vector<SymLitNode*>(); $$->push_back(new SymbolNode($1)); }
| literal { $$ = new std::vector<SymLitNode*>(); $$->push_back(new LiteralNode($1)); }
|	sym_lit_list "," SYMBOL { $$ = $1; $$->push_back(new SymbolNode($3));  }
|	sym_lit_list "," literal { $$ = $1; $$->push_back(new LiteralNode($3)); };

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
  SYMBOL { $$ = new SymbolNode($1); }
| literal_no_sign { $$ = new LiteralNode($1); };

sym_lit_expr:
  "+" sym_lit expr_rest { $$ = $3; $$->push_back({+1, $2}); }
| "-" sym_lit expr_rest { $$ = $3; $$->push_back({-1, $2}); }
| sym_lit expr_rest { $$ = $2; $$->push_back({+1, $1}); };

expr_rest:
  %empty { $$ = new std::vector<SymLitSignNode>(); }
| "+" sym_lit expr_rest { $$ = $3; $$->push_back({+1, $2}); }
| "-" sym_lit expr_rest { $$ = $3; $$->push_back({-1, $2}); };

sections:
  %empty {}
| section sections {};

section:
  SECTION SYMBOL ":" {
    try {
      drv.getCode().beginSection($2);
    } catch(exception& e) { throw yy::parser::syntax_error(drv.location, e.what()); }
  } expressions { };

expressions:
	%empty { }
|	expressions expression { };

expression:
	directive 	{ /* handled in each directive */}
|	label 		  {
  try {
    drv.getCode().addLabel($1);
  } catch(exception& e) { throw yy::parser::syntax_error(drv.location, e.what()); }
}
|	instruction	{
  try {
    drv.getCode().addInstructionDirective($1->getEncoding());
  } catch(exception& e) { throw yy::parser::syntax_error(drv.location, e.what()); }
};

label:
  SYMBOL ":" { $$ = $1; };

directive:
	indep_directive {}
| mem_directive { drv.getCode().addInstructionDirective($1->getEncoding()); delete $1; };

mem_directive:
	SKIP literal { $$ = new SkipDirective($2); }
|	BYTE sym_lit_list {
  ByteWordDirective *dir = new ByteWordDirective(1);
  for (auto n : *($2)) {
    if (n->type() == 'L')
      dir->addLiteral(((LiteralNode*)n)->literal);
    else if (n->type() == 'S')
      dir->addSymbol(((SymbolNode*)n)->symbol);
    delete n;
  }
  delete $2;
  $$ = dir;
}
|	WORD sym_lit_list {
  ByteWordDirective *dir = new ByteWordDirective(2);
  for (auto n : *($2)) {
    if (n->type() == 'L')
      dir->addLiteral(((LiteralNode*)n)->literal);
    else if (n->type() == 'S')
      dir->addSymbol(((SymbolNode*)n)->symbol);
    delete n;
  }
  delete $2;
  $$ = dir;
};

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
