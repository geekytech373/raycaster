# Compiler and flags
CXX := g++
BUILD := build/
TARGET := raycaster-app
SRCS := $(wildcard src/*.cpp)
OBJS := $(patsubst src/%.cpp, $(BUILD)%.o, $(SRCS))
DEPFILES := $(OBJS:.o=.d)

CXXFLAGS := -std=c++17 -Wall -Wextra -Iinclude -O2 -MMD -MP
LDFLAGS :=
INCDIR := include/
LIBS := -lsfml-graphics -lsfml-window -lsfml-system -lX11 -lXrandr \
	-lXcursor -lXinerama -lXxf86vm -lXi -lGL -ldl -lpthread \
	-lrt -ludev -lfreetype

# Default target
all: $(BUILD) $(TARGET)

# Build directory
$(BUILD):
	mkdir -p $@

# Link target
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $^ $(LIBS)

# Compile objects
$(BUILD)%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Include dependencies
-include $(DEPFILES)

# Run target
run: all
	./$(TARGET)

# Clean up
.PHONY: all clean run
clean:
	rm -rf $(BUILD) $(TARGET)
