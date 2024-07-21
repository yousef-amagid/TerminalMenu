#ifndef TERMINAL_MENU_HPP
#define TERMINAL_MENU_HPP

#include <iostream>
#include <vector>
#include <unistd.h>
#include <sstream>

#include "utils.hpp"
#include "colors.hpp"

class TerminalMenu {

    private:
        std::string menuTheme = ANSI_COLOR_RESET;

    public:
        
        TerminalMenu();
        ~TerminalMenu();
        void setTheme(const Theme&, const Theme&);
        int showMenu(std::vector<std::string>, std::string, bool);

};

#endif // TERMINAL_MENU_HPP
