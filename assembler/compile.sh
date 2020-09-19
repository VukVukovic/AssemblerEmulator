mkdir -p build
cd build
flex -o scanner.cpp --header-file=scanner.h ../src/scanner.l
bison -o parser.cpp --define=api.location.file='"location.h"' --defines=parser.h ../src/parser.y
cd ..
g++ -std=c++17 -Wall -o assembler src/*.cpp build/*.cpp -Iinc -Ibuild
rm -r build
