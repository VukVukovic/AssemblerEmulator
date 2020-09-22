#include "terminal.h"
#include "cpu.h"
#include "emulatorexception.h"
#include "memory.h"
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <termios.h>

struct termios terminalBackup;
volatile int killed = 0;

void restoreFlags() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminalBackup);
}

void cleanTerminal(int sig){ 
  restoreFlags();
  killed = 1;
}

void Terminal::setup() {
  if (tcgetattr(STDIN_FILENO, &terminalBackup) < 0) {
    throw EmulatorException("Terminal cannot be started");
  }

  static struct termios raw = terminalBackup;
  raw.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN);
  raw.c_cflag &= ~(CSIZE | PARENB);
  raw.c_cflag |= CS8;
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 0;

  if (atexit(restoreFlags) != 0) {
    throw EmulatorException("Cannot restore terminal");
  }

  atexit(restoreFlags);
  signal(SIGINT, cleanTerminal); 

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)) {
    throw EmulatorException("Terminal cannot be configured");
  }
}

void Terminal::clean() { restoreFlags(); }

void Terminal::readInput() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    buffer.push(c);
  }

  if (buffer.size() > 0 && cpu.canRequest(CPU::TERMINAL_INTERRUPT)) {
    memory.write(DATA_IN, buffer.front(), 1);
    buffer.pop();
    cpu.interruptMark(CPU::TERMINAL_INTERRUPT);
  }

  if (killed) exit(1);
}
