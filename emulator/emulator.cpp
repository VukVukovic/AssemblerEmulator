#include <iostream>
#include "binaryinfile.h"
#include "../chunkheader.h"
#include "../relocations.h"
#include "../symboltable.h"

using namespace std;

char const hex_chars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

string getHex(char byte) {
  string s;
  s += hex_chars[ ( byte & 0xF0 ) >> 4 ];
  s += hex_chars[ ( byte & 0x0F ) >> 0 ];
  return s;
}

string relos[] = { "R_8", "R_16", "R_PC"};
string types[] = { "EXT", "ABS", "REL" };


int main() {
  BinaryInFile file("../file.o");

  while (!file.eof()) {
    ChunkHeader header = file.read<ChunkHeader>();

    if (header.type == SYMBOLS) {
      cout << "------------------" << endl;
      cout << "SYMBOL TABLE " << endl;
      for (int i = 0; i < header.size; i++) {
        cout  << file.read<string>() << "\t" << types[file.read<SymbolType>()]
              << "\t" << file.read<int>() << " (" << file.read<string>() << ")" << endl;
      }
    } else if (header.type == SECTION) {
      cout << "------------------" << endl;
      cout << "SECTION " << file.read<string>() << endl;
      for (int i = 1; i <= header.size; i++) {
        cout << getHex(file.read<char>()) << " ";
        if (i % 10 == 0) cout << endl;
      }
      cout << endl;
    } else if (header.type == RELOCATION) {
      cout << "------------------" << endl;
      cout << "RELOCATION FOR " << file.read<string>() << endl;
      for (int i = 0; i < header.size; i++) {
        cout << relos[file.read<RelocationType>()] << "\t" << file.read<string>() << "\t" << file.read<int>() << endl;
      }
    }
  }

  return 0;
}
