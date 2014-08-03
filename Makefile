CXX = g++
LD =
CXXFLAGS = -Wall -std=c++11
LDFLAGS = -Wall -std=c++11
LDLIBS = -lglfw3 -ldl -lGL -lGLU -lX11 -lXi -lXrandr -lXxf86vm -lpthread -lXcursor

OBJ_DIR = bin
LIB_DIR =
INC_DIR = -I./include/
SRC_DIR = src

SOURCE = $(SRC_DIR)/main.cpp $(SRC_DIR)/gl3w.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/gl3w.o
EXECUTABLE = $(OBJ_DIR)/camo-generator

all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): bin/main.o bin/gl3w.o
	$(CXX) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LDLIBS)

bin/main.o: src/main.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c -o $@ $<

bin/gl3w.o: src/gl3w.c
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c -o $@ $<

init:
	mkdir bin

clean:
	rm -r $(OBJECTS)

dist-clean: clean
	rm -r $(EXECUTABLE)
