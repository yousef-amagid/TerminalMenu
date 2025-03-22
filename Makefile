SRC_DIR = src
SRC_FILES := $(SRC_DIR)/TerminalMenu.cpp
SRC_FILES += $(SRC_DIR)/utils.cpp

INC_DIR = include
INC_FILES := $(INC_DIR)/TerminalMenu.hpp
INC_FILES += $(INC_DIR)/utils.hpp
INC_FILES += $(INC_DIR)/colors.hpp

TEMP_DIR = temp
TEMP_FILES := $(TEMP_DIR)/TerminalMenu.o
TEMP_FILES += $(TEMP_DIR)/libutils.a

EXAMPLE_DIR = test
BUILD_DIR = build

json ?= $(EXAMPLE_DIR)/tree.json

# Phony
.PHONY: all clean

# all
all: example exampleGen

# Object files & static libs
ofiles: $(TEMP_FILES)

$(TEMP_DIR)/TerminalMenu.o: $(SRC_DIR)/TerminalMenu.cpp $(INC_FILES)
	mkdir -p $(TEMP_DIR)
	g++ -c -fPIC $(SRC_DIR)/TerminalMenu.cpp -I $(INC_DIR) -o $(TEMP_DIR)/TerminalMenu.o

$(TEMP_DIR)/libutils.a: $(TEMP_DIR)/utils.o
	ar rcs $(TEMP_DIR)/libutils.a $(TEMP_DIR)/utils.o

$(TEMP_DIR)/utils.o: $(SRC_DIR)/utils.cpp $(INC_DIR)/utils.hpp
	mkdir -p $(TEMP_DIR)
	g++ -c -fPIC $(SRC_DIR)/utils.cpp -I $(INC_DIR) -o $(TEMP_DIR)/utils.o


# Shared library
lib: $(BUILD_DIR)/libTerminalMenu.so
	
$(BUILD_DIR)/libTerminalMenu.so: $(TEMP_FILES)
	mkdir -p $(BUILD_DIR)
	g++ -shared -o $(BUILD_DIR)/libTerminalMenu.so $(TEMP_FILES) -Wl,--exclude-libs,ALL

# Static Example
example: $(BUILD_DIR)/example.out

$(BUILD_DIR)/example.out: lib $(EXAMPLE_DIR)/example.cpp
	g++ $(EXAMPLE_DIR)/example.cpp -I $(INC_DIR) -L$(BUILD_DIR) -lTerminalMenu -o $(BUILD_DIR)/example.out

# Example Generator:
exampleGen: $(BUILD_DIR)/exampleGen.out 

$(BUILD_DIR)/exampleGen.out: lib $(EXAMPLE_DIR)/exampleGen.cpp 
	g++ $(EXAMPLE_DIR)/exampleGen.cpp -I $(INC_DIR) -L$(BUILD_DIR) -lTerminalMenu -o $(BUILD_DIR)/exampleGen.out

$(EXAMPLE_DIR)/exampleGen.cpp: $(json) $(BUILD_DIR)/generateExample.out 
	./$(BUILD_DIR)/generateExample.out $(json) $(EXAMPLE_DIR)/exampleGen.cpp

$(BUILD_DIR)/generateExample.out: $(SRC_DIR)/generateExample.cpp $(INC_DIR)/json.hpp
	g++ $(SRC_DIR)/generateExample.cpp -I $(INC_DIR) -o $(BUILD_DIR)/generateExample.out

# Clean
clean:
	rm -rf $(TEMP_DIR) $(BUILD_DIR) $(EXAMPLE_DIR)/exampleGen.cpp
