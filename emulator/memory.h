#ifndef MEMORY_H
#define MEMORY_H
#define MEM_SIZE 1<<16

#include <vector>
using namespace std;

class Memory {
public:
  static Memory& getInstance() {
    static Memory instance;
    return instance;
  }
  Memory(Memory const&) = delete;
  void operator=(Memory const&)  = delete;

private:
  Memory() : memory(MEM_SIZE, 0) {}
  vector<char> memory;

public:
  void load(int startLocation, const vector<char>& bytes);
};

#endif
