SOURCE_DIR = src
INCLUDE_DIR = include
OBJECT_DIR = obj

SOURCE_FILES = $(wildcard $(SOURCE_DIR)/*.cpp)
HEADERS = $(wildcard $(INCLUDE_DIR)/*.hpp)
OBJECTS = $(patsubst $(SOURCE_DIR)/%.cpp, $(OBJECT_DIR)/%.o, $(SOURCE_FILES))

CPP = g++
CPPFLAGS = -g -Wall -Wextra -fopenmp
IFLAGS = -Iinclude

RM = rm -rf

.PHONY: default run clean

default: main

main: $(OBJECTS)
	$(CPP) $(CPPFLAGS) $^ -o $@

$(OBJECT_DIR)/main.o : $(SOURCE_DIR)/main.cpp $(HEADERS) | $(OBJECT_DIR)
	$(CPP) $(CPPFLAGS) $(IFLAGS) -c $< -o $@

$(OBJECT_DIR)/%.o : $(SOURCE_DIR)/%.cpp $(HEADERS) | $(OBJECT_DIR)
	$(CPP) $(CPPFLAGS) -fPIC $(IFLAGS) -c $< -o $@

$(OBJECT_DIR):
	mkdir -p $@

run: main
	./$^

clean:
	$(RM) main $(OBJECT_DIR)
