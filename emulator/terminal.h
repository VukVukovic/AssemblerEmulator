#ifndef TERMINAL_H
#define TERMINAL_H

class thread;
struct termios;

class Terminal {
  CPU& cpu;
  static void terminalThread(Terminal* terminal);
  thread *thread = nullptr;
  termios originalTerm;

public:
  Terminal(CPU &cpu) : cpu(cpu) {}

  void start()
};
#endif
