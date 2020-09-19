#ifndef EMULATOREXCEPTION_H
#define EMULATOREXCEPTION_H

#include <string>
#include <exception>
using namespace std;

struct EmulatorException : public exception {
    string problem;

    EmulatorException(const string& problem) : problem(problem) {}

    virtual const char* what() const throw() {
        return "Emulator exception";
    }

    string getProblem() const { return problem; }
};

#endif
