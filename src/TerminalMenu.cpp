#include <sstream>
#include <unistd.h>

#include "utils.hpp"
#include "TerminalMenu.hpp"

TerminalMenu::TerminalMenu() {

    getInitialTerminalOptions();
    RegisterSignalHandling();
    setNewTerminalOptions();

}

TerminalMenu::~TerminalMenu() {
            
    clearScreen();
    restoreTerminalOptions();

}

void TerminalMenu::handleBackOption(std::vector<std::string>& options, bool back) {
    // Check if "Back" is at the end of the options vector
    bool isBackPresent = !options.empty() && options.back() == "Back";

    if (back) {
        if (!isBackPresent) {
            options.push_back("Back");
        }
    } else {
        if (isBackPresent) {
            // Remove "Back" if present at the end
            options.pop_back();
        }
    }
}

void TerminalMenu::setTheme(const Theme& fg, const Theme& bg) {
    
    std::stringstream colorSeq;
    colorSeq << ANSI_ESCAPE << "["
             << fg.mode << fg.color << ";"
             << bg.mode << bg.color << "m";
             
    menuTheme = colorSeq.str();
};


int TerminalMenu::showMenu(std::vector<std::string>& options, std::string title = "", bool back = false) {

    handleBackOption(options, back);

    int highlight = 0;
    int selected = -1;
    bool exit = false;
    char* seq;

    int headerLineCount = 1; // no title (leave one empty line)
    int optionCount = options.size();

    if (!title.empty()) {
        headerLineCount++; // empty line after title   
        std::stringstream repl;
        repl << ANSI_COLOR_RESET << "\n " << menuTheme;
        std::string replStr = repl.str();

        size_t pos = 0;
        while ((pos = title.find("\n", pos)) != std::string::npos) {
            title.replace(pos, 1, replStr);
            pos += replStr.length();
            headerLineCount++; // title lines
        }
    }

    int rowCount = getTerminalRowCount() - headerLineCount - 2;
    int start = 0; // Start index of the first visible option
    int end = std::min(optionCount, rowCount); // End index (only shows as many as can fit on the screen)

    while (!exit) {

        clearScreen();
        std::cout  << "\n";
        if (!title.empty()) {
            std::cout
            << " " << menuTheme << title
            << ANSI_COLOR_RESET << std::endl << std::endl;
        }

        // Display only the options that fit within the screen (from start to end)
        for (int i = start; i < end; ++i) {
            if (i == highlight) {
                std::cout
                << " \033[91m" << ">" // bright red
                <<   ANSI_COLOR_RESET << " " // reset
                <<   menuTheme // apply theme
                <<   ANSI_COLOR_INVERT << options[i] // highlight (invert)
                <<   ANSI_COLOR_RESET << std::endl; // reset
            } else {
                std::cout
                <<   ANSI_COLOR_RESET
                << "   "
                << menuTheme << options[i]
                << ANSI_COLOR_RESET << std::endl;
            }
        }

        seq = getKey();

        if (seq[1] == '\0') { // 2nd char is NULL (Not Arrow Key)
            switch (seq[0]) {
                case 27: // 'Esc' 
                case 113: // 'q'
                    exit = true;
                    break;
                case 10: // '\n' ( 'Enter' )
                    if (!(back && highlight == options.size() - 1)) { selected = highlight; }
                    exit = true;
                    break;
                default: break;
            }
        } else {
            if (seq[0] == 27 && seq[1] == 91) { // '\033[' ( Escape Sequence )
                switch (seq[2]) {
                    case 65: // Arrow Up
                        if (highlight > 0) {
                            highlight--;
                        }

                        // Scroll up when the highlight goes above the current top
                        if (highlight < start) {
                            start = highlight;  // Ensure we don't go below the first option
                            end = start + rowCount;
                        }
                        break;

                    case 66: // Arrow Down
                        if (highlight < optionCount - 1) {
                            highlight++;
                        }

                        // Scroll down when the highlight reaches the bottom
                        if (highlight >= end) {
                            start = highlight - rowCount + 1;
                            end = start + rowCount;
                        }
                        break;

                    default: break;
                }
            }
            usleep(10000); // Delay to prevent excessive input speed
        }
    }

    delete[] seq;
    return selected;
}
