CXX = g++
CXXFLAGS = -std=c++20 -Wall -Ilib

# Directories
SRC_DIR = src
BIN_DIR = src

# Target executable
TARGET = $(BIN_DIR)/app.exe

# Source files
SRCS = $(SRC_DIR)/main.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
