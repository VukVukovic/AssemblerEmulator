#ifndef CODE_H
#define CODE_H

#include <map>
#include <vector>
#include <string>
using namespace std;
class Encoding;

class Code {
public:
    map<string,vector<char>> encoded;
    string currentSection;
    void beginSection(const string& section);
    void addInstruction(const Encoding& encoding);
};

#endif