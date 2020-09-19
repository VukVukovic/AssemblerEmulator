#include "terminal.h"
#include "memory.h"
#include "cpu.h"
#include <unistd.h>
#include <iostream>

void Terminal::start() {
  if (started) return;
  started = true;
  tcgetattr(STDIN_FILENO, &originalTerm);
  termios newTerm = originalTerm;
  newTerm.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | CSIZE | PARENB);
  newTerm.c_cflag |= CS8;
  newTerm.c_cc[VMIN]  = 1;
  newTerm.c_cc[VTIME] = 0;
  atexit(Terminal::cleanUpTerminal);
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &newTerm);
  readingThread = thread(reading);
}

void Terminal::reading() {
  while (true) {
    char c = getchar();
    if (Terminal::getInstance().ending) return;
    Terminal::getInstance().memory->write(c, DATA_IN, 1);
    Terminal::getInstance().cpu->interruptMark(3);
  }
}

void Terminal::cleanUpTerminal() {
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &(Terminal::getInstance().originalTerm));
}

void Terminal::stop() {
  if (!started) return;
  ending = true;
  //cout << "Terminal simulation finished. Press any key to continue...";
  cleanUpTerminal();
}

void Terminal::newData() {
  char value = memory->read(DATA_OUT, 1);
  cout << value;
}
