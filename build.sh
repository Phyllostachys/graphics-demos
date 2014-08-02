g++ -Wall -std=c++11 -o bin/main.o -c src/main.cpp -I./include
g++ -Wall -std=c++11 -o bin/gl3w.o -c src/gl3w.c -I./include
g++ -Wall -std=c++11 bin/main.o bin/gl3w.o -lglfw3 -ldl -lGL -lGLU -lX11 -lXi -lXrandr -lXxf86vm -lpthread -lXcursor -o bin/camo-generator
