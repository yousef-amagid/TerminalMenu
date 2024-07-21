#ifndef COLORS_HPP
#define COLORS_HPP

#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_INVERT "\033[7m"

#define ANSI_FG_STNDRD 3
#define ANSI_BG_STNDRD 4
#define ANSI_FG_BRIGHT 9
#define ANSI_BG_BRIGHT 10

enum Color {

    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE

};

struct Theme {
    int mode;
    Color color;
};

#endif // COLORS_HPP
