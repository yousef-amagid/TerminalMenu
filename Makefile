SOURCE_DIR = src
SOURCE_FILES := $(SOURCE_DIR)/TerminalMenu.cpp
SOURCE_FILES += $(SOURCE_DIR)/utils.cpp

INC_DIR = include
INC_FILES := $(INC_DIR)/TerminalMenu.hpp
INC_FILES += $(INC_DIR)/utils.hpp
INC_FILES += $(INC_DIR)/colors.hpp

TEMP_DIR = temp
TEMP_FILES := $(TEMP_DIR)/TerminalMenu.o
TEMP_FILES += $(TEMP_DIR)/libutils.a

EXAMPLE_DIR = test
BUILD_DIR = build

# Phony
.PHONY: all clean

# all
all: example

# Object files & static libs
ofiles: $(TEMP_FILES)

$(TEMP_DIR)/TerminalMenu.o: $(SOURCE_DIR)/TerminalMenu.cpp $(INC_FILES)
	mkdir -p $(TEMP_DIR)
	g++ -c -fPIC $(SOURCE_DIR)/TerminalMenu.cpp -I $(INC_DIR) -o $(TEMP_DIR)/TerminalMenu.o


$(TEMP_DIR)/libutils.a: $(TEMP_DIR)/utils.o
	ar rcs $(TEMP_DIR)/libutils.a $(TEMP_DIR)/utils.o

$(TEMP_DIR)/utils.o: $(SOURCE_DIR)/utils.cpp $(INC_DIR)/utils.hpp
	mkdir -p $(TEMP_DIR)
	g++ -c -fPIC $(SOURCE_DIR)/utils.cpp -I $(INC_DIR) -o $(TEMP_DIR)/utils.o


# Shared library
lib: $(BUILD_DIR)/libTerminalMenu.so
	
$(BUILD_DIR)/libTerminalMenu.so: $(TEMP_FILES)
	mkdir -p $(BUILD_DIR)
	g++ -shared -o $(BUILD_DIR)/libTerminalMenu.so $(TEMP_FILES) -Wl,--exclude-libs,ALL

# Example:
example: $(BUILD_DIR)/example.out

$(BUILD_DIR)/example.out: $(EXAMPLE_DIR)/example.cpp $(BUILD_DIR)/libTerminalMenu.so
	g++ $(EXAMPLE_DIR)/example.cpp -I $(INC_DIR) -L$(BUILD_DIR) -lTerminalMenu -o $(BUILD_DIR)/example.out

# Clean
clean:
	rm -rf $(TEMP_DIR) $(BUILD_DIR)
