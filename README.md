# Terminal Menu
Interactive terminal menu in C++ for Linux.  
**Note:** Tested on Ubuntu 18.04.

![""](screenshot.png)

## Usage
Change your selection with the "Up" and "Down" arrow keys.
Press "Enter" to confirm,
and "Esc" or "Q" to exit the menu.
In the current state, the following features are available:

1. Add single or multi line title to menu.
2. Set color theme (text and background colors).
3. Add "Back" to the list of options (for nested menus).
4. Generate a fully functional menu tree using a JSON file.

### Upcoming Features
1. Search through the menu.
2. Optional clear screen (currently hard-coded to clear it).

## Build & Run Examples
Steps to build the library (.so) and the included example.
You can also use "make lib" and "make example" to build the library and example individually. 


```bash
make 
export LD_LIBRARY_PATH=$(pwd)/build:$LD_LIBRARY_PATH
./build/example.out
```

### Generating Example using JSON Structure

You can generate code for and build a fully functional nested menu tree using "make exampleGen". The default JSON file used is **test/tree.json**. You can also create your own file and pass it explicitly to the make command.

```bash
make exampleGen json=<jsonFile>
./build/exampleGen.out
```
Empty attributes in the JSON default to the values listed below. If you wish to further edit your menu tree's actions, you can do so in **test/exampleGen.cpp** before rebuilding the target.

- **back:** Defaults to **false**. 
- **title:** Defaults to empty string (""). 
- **action:** Defaults to exiting the entire tree.
- **Theme Attributes:** Default to **WHITE** on **BLACK** (more info below).

## Functions

```C++
int TerminalMenu::showMenu(std::vector<std::string>& options, std::string title = "", bool back = false)
```

- **Return:** Index of the option selected *[0, n]* or **-1** if no option is selected.
- **std::vector\<std::string\>& options:** List of options in the menu.
- **std::string title:** *OPTIONAL* Menu title that appears above the list of options
- **bool back:** *OPTIONAL* Adds a "Back" button to **options** if set to **true**.

```C++
void TerminalMenu::setTheme(const Theme& fg, const Theme& bg)
```

- **const Theme& fg:** foreground (text) color info.
- **const Theme& bg:** background color info.
- **struct Theme:** 8 basic colors x 2 brightness levels = 16 supported colors.

```C++
// from "colors.hpp":
struct Theme {
    int mode;
    Color color;
};
```
- **Theme::(int)mode:**

    - ANSI_FG_STNDRD
    - ANSI_BG_STNDRD
    - ANSI_FG_BRIGHT
    - ANSI_BG_BRIGHT

- **Theme::(Color)color:**

    - BLACK
    - RED
    - GREEN
    - YELLOW
    - BLUE
    - MAGENTA
    - CYAN
    - WHITE
