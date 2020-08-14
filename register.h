#ifndef REGISTER_H
#define REGISTER_H

#include <iostream>
using namespace std;

class Register {
    char regNum;
    bool high = false;
    int size = 2;

public:
    Register(string reg);
    Register() = default;

    char getEncoding() const { return regNum; }
    bool getLH() const { return high; }
    int getSize() const { return size; }

    friend ostream& operator<<(ostream& os, const Register& reg) {
        return os << "REG-" << reg.regNum;
    }
};
#endif