#include <iostream>
#include "code.h"
#include "encoding.h"

void Code::beginSection(const string& section) {
    currentSection = section;
    encoded[currentSection] = {};
    cout << "NEW SECTION " << currentSection << endl;
}

void Code::addInstruction(const Encoding& encoding) {
    cout << "ADDING" << encoding.bytes.size() << endl;
    encoded[currentSection].insert(encoded[currentSection].end(), encoding.bytes.begin(), encoding.bytes.end());
}