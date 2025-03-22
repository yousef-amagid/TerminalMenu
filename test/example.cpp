#include "TerminalMenu.hpp"
#include <vector>

int main() {

    bool exit_D00 = false;
    bool exit_D10 = false;
    bool exit_D11 = false;

    std::vector<std::string> list_0 = {
        "Option 1",
        "Option 2",
        "Option 3",
        "Option 4",
    };

    std::vector<std::string> list_1 = {
        "Option A",
        "Option B",
        "Option C"
    };

    std::vector<std::string> list_2 = {
        "Option I",
        "Option II",
        "Option III"
    };

    std::string title_D00 = "Hell Yeah!!";
    std::string title_D10 = "The title works fine.\nMultiple lines too :)";
    std::string title_D11 = "The back option too :D";

    TerminalMenu mm;

    Theme fg;
    fg.mode = ANSI_FG_STNDRD;
    fg.color = WHITE;

    Theme bg;
    bg.mode = ANSI_BG_STNDRD;
    bg.color = BLACK;

    mm.setTheme(fg, bg);
    int selected;

    while (!exit_D00) {

        exit_D10 = false;
        exit_D11 = false;
        
        selected = mm.showMenu( list_0, title_D00, false );
        switch ( selected ) {
            case -1:
                exit_D00 = true;
                break;
            case 0:

                while (!exit_D10) {

                    selected = mm.showMenu( list_1, title_D10, true );
                    switch ( selected ) {
                        
                        case -1:
                            exit_D10 = true;
                            break;
                        default:
                            exit_D10 = true;
			                exit_D00 = true;
                            break;          
                    } 
                }

                break;

            case 2:
            
                while (!exit_D11) {

                    selected = mm.showMenu( list_2, title_D11, true );
                    switch ( selected ) {
                        
                        case -1:
                            exit_D11 = true;
                            break;
                        default:
                            exit_D11 = true;
                            exit_D00 = true;
                            break;

                    }
                }

                break;

	        default:
		        exit_D00 = true;
                break;
        }
    }

    return 0;
    
}
