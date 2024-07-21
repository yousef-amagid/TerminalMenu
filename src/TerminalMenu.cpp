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

void TerminalMenu::setTheme(const Theme& fg, const Theme& bg) {
    
    std::stringstream colorSeq;
    colorSeq << ANSI_ESCAPE << "["
             << fg.mode << fg.color << ";"
             << bg.mode << bg.color << "m";
             
    menuTheme = colorSeq.str();
};


int TerminalMenu::showMenu(std::vector<std::string> options, std::string title = "", bool back = false) {

    int highlight = 0;
    int selected = -1;
    bool exit = false;
    char* seq;    
    
    if ( back ) { // && options[options.size() - 1] != "Back" ) {
        options.push_back("Back");
    }

    if (!title.empty()) {    
        std::stringstream repl;
        repl << ANSI_COLOR_RESET << "\n " << menuTheme;
        std::string replStr = repl.str();

        size_t pos = 0;
        // Loop through the string to find and replace all occurrences of '\n'
        while ((pos = title.find("\n", pos)) != std::string::npos) {
            title.replace(pos, 1, replStr);
            pos += replStr.length(); // Move to the next position after the replacement
        }

    }


    while (!exit) {

        clearScreen();
        std::cout  << "\n";
        if (!title.empty()) {
        
           
            std::cout
            << " " << menuTheme << title
            << ANSI_COLOR_RESET << std::endl << std::endl;
        
        } 

        for (size_t i = 0; i < options.size(); ++i) {
            if (i == highlight) { 
                                
                std::cout
                << " \033[91m" << ">" // red
                <<   ANSI_COLOR_RESET << " " // reset
                <<   menuTheme // apply theme
                <<   ANSI_COLOR_INVERT << options[i] // highlight (invert)
                <<   ANSI_COLOR_RESET << std::endl; // reset

            } else {
                
                std::cout
                << "   " 
                << menuTheme << options[i]
                << ANSI_COLOR_RESET << std::endl; 
            
            }
        }

        seq = getKey();

        if ( seq[1] == '\0' ) { // 2nd char is NULL (Not Arrow Key)
            switch( seq[0] ) {
                case 27: // 'Esc' 
                case 113: // 'q'
                    exit = true;
                    break;
                case 10: // '\n' ( 'Enter' )
                    if ( !(back && highlight == options.size() -1) ) { selected = highlight; }
                    else {}
                    exit = true;
                    break;
                default: break;
            }
        } else {
            if ( seq[0] == 27 && seq[1] == 91 ) { // '\033[' ( Escape Sequence )
                switch( seq[2] ) {
                    case 65: // Arrow Up
                        highlight = (highlight - 1 + options.size()) % options.size();
                        break;
                    case 66: // Arrow Down
                        highlight = (highlight + 1) % options.size();
                        break;
                    default: break;
                }

            } usleep(10000);
        } 
    }
    
    delete[] seq;
    return selected;
}
