#include "terminal.h"

#include <termios.h>
#include <unistd.h>

void Terminal::start() {

}

static void Terminal::terminalThread(Terminal* terminal) {
  tcgetattr(STDIN_FILENO, &(terminal->originalTerm));
  termios raw = orig_termios;
  raw.c_lflag &= ~(ECHO | ECHONL | ICANON | IEXTEN | CSIZE | PARENB);
  raw.c_cflag |= CS8;
  raw.c_cc[VMIN]  = 1;
  raw.c_cc[VTIME] = 0;

  atexit(Terminal::cleanup_terminal);

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

    sem_init(&input_lock, 0, 0);
    input_thread = new std::thread(input_run);
}
