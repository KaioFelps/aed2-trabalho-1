TARGET = indexer
BUILD_DIR = build
INCLUDE_DIRS = -Iinclude

SUBDIRS = $(shell find src -type d)
SRC_FILES = $(foreach dir, $(SUBDIRS), $(wildcard $(dir)/*.cpp))

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra

all: $(BUILD_DIR)/$(TARGET)

# cria o diretório de build e compila os objetos e o executável
$(BUILD_DIR)/$(TARGET): $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(INCLUDE_DIRS) $(SRC_FILES) -o $@

clean:
	@rm -rf $(BUILD_DIR)

.PHONY: all clean
