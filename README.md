## graphics-demos ##

A repo to store and showcase any graphics demos that I make.

### Libraries ###
- [glad](https://github.com/Dav1dde/glad) - to get the latest and greatest in OpenGL
- [glfw](https://github.com/glfw/glfw) - for window context creation
- [glm](https://github.com/g-truc/glm) - for some sweet GLSL maths

### Build ###
To build, run in Windows cmd/PS or *nix shell:
```shell
git submodule init
git submodule update
mkdir build
cd build
cmake ../
```
From there, run either `make` or open Visual Studios to compile.
