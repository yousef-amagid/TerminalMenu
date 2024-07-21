#include "TerminalMenu.hpp"
#include <vector>

int main() {

    bool exit_0 = false;
    bool exit_1 = false;
    bool exit_2 = false;

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


    std::string title_0 = "Hell Yeah!!";
    std::string title_1 = "The title works fine.\nMultiple lines too :)";
    std::string title_2 = "The back option too :D";


    TerminalMenu mm;

    // Theme fg;
    // fg.mode = ANSI_FG_STNDRD;
    // fg.color = BLUE;

    // Theme bg;
    // bg.mode = ANSI_BG_BRIGHT;
    // bg.color = YELLOW;

    // mm.setTheme(fg, bg);
    int selected;

    while (!exit_0) {

        exit_1 = false;
        exit_2 = false;
        
        selected = mm.showMenu( list_0, title_0, false );
        switch ( selected ) {
            case -1:
                exit_0 = true;
                break;
            case 0:

                while (!exit_1) {

                    selected = mm.showMenu( list_1, title_1, true );
                    switch ( selected) {
                        
                        case -1:
                            exit_1 = true;
                            break;
                        case 0:
                        case 1:
                        case 2:
                            exit_1 = true;
			    exit_0 = true;
                        
                    }
                } break;

            case 1:
                exit_0 = true;
                break;
            case 2:
                 while (!exit_2) {

                    selected = mm.showMenu( list_2, title_2, true );
                    switch ( selected) {
                        
                        case -1:
                            exit_2 = true;
                            break;
                        case 0:
                        case 1:
                        case 2:
                            exit_2 = true;
                            exit_0 = true;
                        
                    }
                 } break;
	    case 3:
		exit_0 = true;
        }

    }


    return 0;
    
}
