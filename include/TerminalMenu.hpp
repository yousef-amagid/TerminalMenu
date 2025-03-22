#ifndef TERMINAL_MENU_HPP
#define TERMINAL_MENU_HPP

#include <iostream>
#include <vector>
#include "colors.hpp"

class TerminalMenu {

    private:
        std::string menuTheme = ANSI_COLOR_RESET;
        void handleBackOption(std::vector<std::string>&, bool);

    public:
        
        TerminalMenu();
        ~TerminalMenu();
        void setTheme(const Theme&, const Theme&);
        int showMenu(std::vector<std::string>&, std::string, bool);

};

#endif // TERMINAL_MENU_HPP
