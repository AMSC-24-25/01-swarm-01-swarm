# Variabili
CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Iinclude -fopenmp
LDFLAGS = -fopenmp

# Directory
SRC_DIR = src
INCLUDE_DIR = include
BUILD_DIR = build
TARGET = $(BUILD_DIR)/main

# File sorgenti e oggetti
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Regole principali
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR)
clean images:
	rm ./animation/*.csv ./frames/*.png
	rm pso_animation.gif
# Regole di utilità
run: $(TARGET)
	./$(TARGET)
