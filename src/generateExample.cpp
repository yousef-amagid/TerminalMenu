#include <iostream>
#include <fstream>
#include <vector>
#include <list>

#include "json.hpp"
using json = nlohmann::json;

int tabs = 1;
char indent = '\t';

// Escape newline character ('\n') instances in multiline titles
std::string escapeNewLines(const std::string& input) {
    std::string result;
    for (char c : input) {
        if (c == '\n') {
            result += "\\n"; 
        } else {
            result += c;
        }
    }
    return result;
}

// Function to generate the menu code
void generateMenuCode(const json& menuObj, std::ofstream& outputFile, std::vector<std::string>& parentChain) {

    // Extract menu id
    std::string id = menuObj["id"];

    // Add parent to the parent chain
    parentChain.push_back(menuObj["parent"]);

    // Generate menu exit, title, and option list definitions
    outputFile << std::string(tabs, indent) << "bool exit_" << id << " = false;\n";
    outputFile << std::string(tabs, indent) << "std::string title_" << id << " = \"" << escapeNewLines(menuObj.value("title", "")) << "\";\n";
    outputFile << std::string(tabs, indent) << "std::vector<std::string> list_" << id << " = {\n";
    
    tabs++;
    for (std::string label: menuObj["options"]["labels"]) {
        outputFile << std::string(tabs, indent) << "\"" << label << "\",\n";
    }

    outputFile << std::string(--tabs, indent) << "};\n\n";

    // Generate current menu's logic loop
    outputFile << std::string(tabs, indent) << "while (!exit_" << id << ") {\n\n";
    outputFile << std::string(++tabs, indent) << "selected = mm.showMenu(list_" << id << ", title_" << id << ", " << (menuObj.value("back", false) ? "true" : "false") << ");\n";
    outputFile << std::string(tabs, indent) << "switch (selected) {\n";

    outputFile << std::string(++tabs, indent) << "case -1:\n"; // -1 case
    outputFile << std::string(++tabs, indent) << "exit_" << id << " = true;\n";
    outputFile << std::string(tabs, indent) << "break;\n";

    tabs--;
    int caseIndex = 0;
    for (const auto& action : menuObj["options"]["actions"]) {

        // Generate code for handling menu actions
        if (action.is_object() && action != "default") {

            // If action is another menu, generate code recursively
            outputFile << std::string(tabs++, indent) << "case " << caseIndex << ": {\n\n";
            generateMenuCode(action, outputFile, parentChain); // Pass parentChain along
            outputFile << std::string(tabs, indent) << "break;\n\n";
            outputFile << std::string(--tabs, indent) << "}\n";
        }
        caseIndex++;
    }

    // Generate code for default case, switch case exit, and logic loop exit
    outputFile << std::string(tabs, indent) << "default:\n";
    outputFile << std::string(++tabs, indent) << "exit_" << id << " = true;\n";

    // Trace parent exits for defaults
    for (int i = parentChain.size() - 1; i > 0; i--) {
        outputFile << std::string(tabs, indent) << "exit_" << parentChain[i] << " = true;\n";
    }

    // Remove current parent from chain
    parentChain.pop_back();

    outputFile << std::string(tabs, indent) << "break;\n";

    tabs--;
    outputFile << std::string(--tabs, indent) << "}\n"; // Close switch
    outputFile << std::string(--tabs, indent) << "}\n"; // Close while loop
}

int main(int argc, char* argv[]) {
    // Check if correct number of arguments are passed
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <jsonMenuStructure> <cppOutput>" << std::endl;
        return 1;
    }

    // Open the input JSON file
    std::ifstream ifs(argv[1]);
    if (!ifs) {
        std::cerr << "Error opening input JSON file: " << argv[1] << std::endl;
        return 1;
    }

    json menuData;
    ifs >> menuData;

    // Create an output file where we will write the C++ code
    std::ofstream outputFile(argv[2]);
    if (!outputFile) {
        std::cerr << "Error opening output C++ file: " << argv[2] << std::endl;
        return 1;
    }

    // Write the necessary includes at the top of the generated file
    outputFile << "#include \"TerminalMenu.hpp\"\n";
    outputFile << "#include <vector>\n\n";

    // Write the main function that uses the generated menus
    outputFile << "int main() {\n\n";
    outputFile << std::string(tabs, indent) << "TerminalMenu mm;\n";
    outputFile << std::string(tabs, indent) << "int selected;\n\n";

    std::string fgMode = menuData["menu"]["theme"]["foreground"]["mode"];
    fgMode = (fgMode != "default" ? fgMode : "STNDRD");
    std::string fgColor = menuData["menu"]["theme"]["foreground"]["color"];
    
    std::string bgMode = menuData["menu"]["theme"]["background"]["mode"];
    bgMode = (bgMode != "default" ? bgMode : "STNDRD");
    std::string bgColor = menuData["menu"]["theme"]["background"]["color"];

    outputFile <<  std::string(tabs, indent) << "Theme fg;\n";
    outputFile <<  std::string(tabs, indent) << "fg.mode = ANSI_FG_" << fgMode << ";\n";

    if (fgColor != "default") outputFile <<  std::string(tabs, indent) << "fg.color = " << fgColor << ";\n\n";

    outputFile <<  std::string(tabs, indent) << "Theme bg;\n";
    outputFile <<  std::string(tabs, indent) << "bg.mode = ANSI_BG_" << bgMode << ";\n";
    if (bgColor != "default") outputFile <<  std::string(tabs, indent) << "bg.color = " << bgColor << ";\n\n";

    outputFile <<  std::string(tabs, indent) << "mm.setTheme(fg, bg);\n\n";

    // Generate the function definitions for the menus
    std::vector<std::string> parentChain;  // Initialize the parent chain
    generateMenuCode(menuData["menu"], outputFile, parentChain);

    outputFile << "\n";
    outputFile << std::string(tabs, indent) << "return 0;\n\n";
    outputFile << std::string(--tabs, indent) << "}\n";

    return 0;
}
