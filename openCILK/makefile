# Variables for the compiler and modules
CXX = g++
CXXFLAGS = -fcilkplus
TARGET = merge_sort
SRC = merge_sort.c

# Default target to build the program
all: $(TARGET)

# Target to compile the program
$(TARGET): $(SRC)
	@echo "Loading required modules..."
	module load gcc/7.2.0
	module load mpich/3.2.1-gcc-7.2.0
	module load cmake/3.17.2
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# Clean up the compiled files
clean:
	rm -f $(TARGET)