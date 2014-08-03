CC = clang
CXX = clang++
CFLAGS = -Wall
CXXFLAGS = -Wall -std=c++11

LD =
LDFLAGS = -Wall -std=c++11
LDLIBS = -lglfw3 -ldl -lGL -lGLU -lX11 -lXi -lXrandr -lXxf86vm -lpthread -lXcursor

SRC_DIR = src
INC_DIR = -I./include/ -I./src/
LIB_DIR =
OBJ_DIR = bin

SOURCE = $(SRC_DIR)/gl3w.c $(SRC_DIR)/main.cpp $(SRC_DIR)/shader_util.cpp
OBJECTS = $(OBJ_DIR)/gl3w.o $(OBJ_DIR)/main.o $(OBJ_DIR)/shader_util.o
EXECUTABLE = $(OBJ_DIR)/camo-generator

.PHONY: all
all: $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LDLIBS)
	cp -u $(SRC_DIR)/vert_shader.vert $(OBJ_DIR)/vert_shader.vert
	cp -u $(SRC_DIR)/frag_shader.frag $(OBJ_DIR)/frag_shader.frag

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) $(INC_DIR) -c -o $@ $<

$(OBJ_DIR)/gl3w.o: $(SRC_DIR)/gl3w.c
	$(CC) $(CFLAGS) $(INC_DIR) -c -o $@ $<

.PHONY: init
init:
	mkdir bin

.PHONY: format
format:
	astyle -A10SNYpHUk1W3OcnrQz1 *.h
	astyle -A10SNYpHUk1W3OcnrQz1 *.c
	astyle -A10SNYpHUk1W3OcnrQz1 *.cpp

.PHONY: clean
clean:
	rm -rf $(OBJECTS)

.PHONY: dist-clean
dist-clean: clean
	rm -rf vert_shader.vert
	rm -rf frag_shader.frag
	rm -rf $(EXECUTABLE)
