#ifndef ASSEMBLEREXCEPTION_H
#define ASSEMBLEREXCEPTION_H

#include <exception>
#include <string>
using namespace std;

struct AssemblerException : public exception {
  string problem;

  AssemblerException(const string &problem) : problem(problem) {}

  virtual const char *what() const throw() { return "Assembler exception"; }

  string getProblem() const { return problem; }
};

#endif
