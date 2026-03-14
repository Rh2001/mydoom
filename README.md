Very simple DOOM engine called mydoom. This is not intended to be an actual fork of the DOOM Engine and it's a recreation I'm doing from scratch just for fun. 

To build it, download everything and run the following command from the base directory: g++ src\main.cpp src\Renderer.cpp src\Shader.cpp src\Map.cpp src\gl.c `
  -Iinclude `
  -IC:\msys64\mingw64\include `
  -LC:\msys64\mingw64\lib `
  -lglfw3 -lSDL2 -lSDL2_mixer -lopengl32 -lgdi32 `
  -o mydoom.exe


You need a C++ compiler, I used mingw64 which you can install and build from MSYS2: https://www.msys2.org/
