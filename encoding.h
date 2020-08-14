#ifndef ENCODING_H
#define ENCODING_H
#include <vector>
#include <string>
using namespace std;

struct SymbolEncodingEntry {
    string symbol;
    int location;
    int size;
    SymbolEncodingEntry(const string& symbol, int location, int size) : symbol(symbol), location(location), size(size) {}
};

struct Encoding {
    int offset = 0;
    vector<char> bytes;
    vector<SymbolEncodingEntry> symbolLocations;
    void add(const Encoding& encoding);
    //Encoding() = default;
    Encoding(const vector<char>& bytes) : bytes(bytes) {}
    Encoding(const vector<char>& bytes, const SymbolEncodingEntry& see) : bytes(bytes), symbolLocations({see}) {}
};
#endif