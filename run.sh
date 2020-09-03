flex -o scanner.cpp scanner.ll
bison -o parser.cc parser.yy
g++ -std=c++17 -g driver.cc main.cc scanner.cpp parser.cc instruction.cpp addressing.cpp register.cpp encoding.cpp code.cpp symboltable.cpp memdirective.cpp equtable.cpp -o main
