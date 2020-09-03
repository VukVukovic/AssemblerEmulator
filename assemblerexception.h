#ifndef ASSEMBLEREXCEPTION_H
#define ASSEMBLEREXCEPTION_H

#include <string>
#include <exception>
using namespace std;

struct AssemblerException : public exception {
    string problem;

    AssemblerException(const string& problem) : problem(problem) {}

    virtual const char* what() const throw() {
        return "Assembler exception";
    }

    const string& getProblem() const { return problem; }
};

#endif
