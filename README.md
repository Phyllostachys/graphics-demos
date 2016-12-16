## graphics-demos ##

A repo to store and showcase any graphics demos that I make.

### Libraries ###
- [glad](https://github.com/Dav1dde/glad) - to get the latest and greatest in OpenGL
- [glfw](https://github.com/glfw/glfw) - for window context creation
- [glm](https://github.com/g-truc/glm) - for some sweet GLSL maths

### Build ###
It used to be easy because I used cmake but that is in flux now... Many folders have windows build scripts, anything else probably doesn't work anymore but the programs are only a few C or C++ files so they can be hand compiled with cl.exe or g++:
```
g++ -Wall some_file.cpp -o some_file # add in libraries as needed.
```
TODO: Fixup build instructions. :-/
