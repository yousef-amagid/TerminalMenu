#ifndef UTILS_HPP
#define UTILS_HPP

#define ANSI_CLEAR_SCREEN "\033[2J\033[1;1H"
#define ANSI_HIDE_CURSOR "\033[?25l"
#define ANSI_SHOW_CURSOR "\033[?25h"
#define ANSI_ESCAPE "\033"
#define BUFFER_SIZE 3

void getInitialTerminalOptions();
void restoreTerminalOptions();
void setNewTerminalOptions();
int getTerminalRowCount();

void clearScreen();
char* getKey();

void signalHandler(int);
void RegisterSignalHandling();

#endif // UTILS_HPP
