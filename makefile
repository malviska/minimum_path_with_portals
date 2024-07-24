# Makefile

# Compiler options
CXX = gcc
CXXFLAGS = -std=c11 -Wall -g -I ./include
CXXFAST = -Ofast -finline-functions -ftree-loop-optimize
# Source files
SRCS := $(wildcard ./src/*.c)

# Object files
OBJS := $(patsubst ./src/%.c,./obj/%.o,$(SRCS))
LINK_OBJS := $(filter-out ./obj/alternative_main.o, $(OBJS)) # Adjacency List
LINK_OBJS_MATRIX := $(filter-out ./obj/main.o, $(OBJS)) # Adjacency Matrix

# Target executable
TARGET := ./bin/tp2.out # Adjacency List
MATRIX := ./bin/matrix.out # Adjacency Matrix

all: $(TARGET) # Run Adjacency List implementation

matrix: $(MATRIX) # Run Adjacency Matrix

$(TARGET): $(LINK_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

$(MATRIX): $(LINK_OBJS_MATRIX)
	$(CXX) $(CXXFLAGS) -o $@ $^ -lm

./obj/%.o: ./src/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
