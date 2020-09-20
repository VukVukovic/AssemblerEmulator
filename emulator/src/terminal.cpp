#include "terminal.h"
#include <unistd.h>
#include <fcntl.h>
#include "emulatorexception.h"
#include "cpu.h"
#include "memory.h"
#include <iostream>

void Terminal::setup() {
  if (tcgetattr(STDIN_FILENO, &terminalBackup) < 0) {
    throw EmulatorException("Terminal cannot be started");
  }

  static struct termios raw = terminalBackup;
  raw.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN);
  raw.c_cflag &= ~(CSIZE | PARENB);
  raw.c_cflag |= CS8;
  raw.c_cc[VMIN]  = 1;
  raw.c_cc[VTIME] = 0;

  if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw)) {
    throw EmulatorException("Terminal cannot be started");
  }

  stdinFlags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, stdinFlags | O_NONBLOCK);
}

void Terminal::clean() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &terminalBackup);
  fcntl(STDIN_FILENO, F_SETFL, stdinFlags);
}

void Terminal::readInput() {
  char c;
  if (read(STDIN_FILENO, &c, 1) == 1) {
    buffer.push(c);
  }

  if (buffer.size()>0 && cpu.canRequest(CPU::TERMINAL_INTERRUPT)) {
    memory.write(DATA_IN, buffer.front(), 1);
    buffer.pop();
    cpu.interruptMark(CPU::TERMINAL_INTERRUPT);
  }
}
