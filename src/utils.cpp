#include <iostream>
#include <csignal>
#include <termios.h>
#include <sys/ioctl.h>

#include "utils.hpp"

static termios oldt;
static termios newt;

void getInitialTerminalOptions() {
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
}

void clearScreen() {
    std::cout << ANSI_CLEAR_SCREEN;
}

void restoreTerminalOptions() {
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    // clearScreen();
    std::cout << ANSI_SHOW_CURSOR;
}

void setNewTerminalOptions() {
    newt.c_lflag &= ~( ICANON | ECHO ); // Disable canonical and echo mode
    newt.c_cc[VMIN] = 1; // At least one key press
    newt.c_cc[VTIME] = 0; // Infinite time
    // newt.c_cc[VQUIT] = '\033';
    tcsetattr(STDIN_FILENO, TCSANOW, &newt); // Apply now
    std::cout << ANSI_HIDE_CURSOR;
}

int getTerminalRowCount() {
    struct winsize terminalSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminalSize);
    return terminalSize.ws_row;
}

char* getKey() {
    char* buf = new char[BUFFER_SIZE];
    // std::memset(buf, 0, BUFFER_SIZE); // Initialize the buffer with null characters
    ssize_t bytesRead = read(STDIN_FILENO, buf, BUFFER_SIZE);
    buf[bytesRead] = '\0';
    return buf;
}

void signalHandler(int signum) {
    clearScreen();
    restoreTerminalOptions();
    exit(signum);
}

void RegisterSignalHandling() {
    signal(SIGTSTP, signalHandler);
    signal(SIGINT, signalHandler);
    signal(SIGTERM, signalHandler);
    signal(SIGQUIT, signalHandler);
}
