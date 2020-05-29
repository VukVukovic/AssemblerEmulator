flex -o scanner.cpp scanner.ll 
bison -o parser.cc parser.yy
g++ driver.cc main.cc scanner.cpp parser.cc -o main