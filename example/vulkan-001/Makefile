# Compiler and flags
CXX = g++
CPP_VERSION_FLAG = -std=c++20
CXXFLAGS = $(CPP_VERSION_FLAG) -fdiagnostics-color=always -g -pedantic-errors -Wall -Weffc++ -Wconversion -Wsign-conversion -Werror
LDFLAGS = -L/home/mrzli/projects/games/vulkan-sdk/x86_64/lib
LIB = -lvulkan -lglfw

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = $(BUILD_DIR)/bin
OBJ_DIR = $(BUILD_DIR)/obj

# Output executable
TARGET = $(BIN_DIR)/vulkan-example

# Source files
SOURCES = $(shell find $(SRC_DIR) -name '*.cpp')
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Default target
.PHONY: all
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CXX) $(OBJECTS) $(CPP_VERSION_FLAG) $(LDFLAGS) $(LIB) -o $@

# Compile source files to object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Create directories if they don't exist
$(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR):
	mkdir -p $@

# Clean build artifacts
.PHONY: clean
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: directories
directories: $(BUILD_DIR) $(OBJ_DIR) $(BIN_DIR)

# Run the executable
.PHONY: run
run: $(TARGET)
	$(TARGET)
