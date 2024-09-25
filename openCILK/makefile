# Variables for the compiler and modules
CXX = g++
CXXFLAGS = -fcilkplus
TARGET = merge_sort
SRC = merge_sort.c

# Default target to build the program
all: $(TARGET)

# Target to compile the program
$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean up the compiled files
clean:
	rm -f $(TARGET)
