# Simply3D_Template

Simply3D_Template is a C++ project template for creating 3D applications using custom code along with several external libraries, including GLEW, GLFW, Assimp, ImGui, and stb_image. Note that libraries included in the `Libraries` folder are external dependencies and are not part of the Simply3D engine itself; they are provided in this repository to simplify the setup process.

## Table of Contents
- [Setup and Prerequisites](#setup-and-prerequisites)
- [Compilation Instructions](#compilation-instructions)

## Setup and Prerequisites

To compile and run this project, the following libraries must be installed on an X11-based system (such as Linux). These libraries can typically be installed using your system’s package manager.

### Required Libraries
- **Assimp**: `libassimp-dev`
- **GLEW**: `libglew-dev`
- **GLFW**: `libglfw3-dev`
- **GLM**: `libglm-dev`
- **X11 and Extensions**: `libx11-dev`, `libxrandr-dev`, `libxi-dev`, `libxinerama-dev`, `libxcursor-dev`
- **OpenGL**: `libgl1-mesa-dev`

To install these libraries on a Debian-based system, run:
```bash
sudo apt update
sudo apt install libassimp-dev libglew-dev libglfw3-dev libglm-dev libx11-dev libxrandr-dev libxi-dev libxinerama-dev libxcursor-dev libgl1-mesa-dev
g++ -o my_program main.cpp Simply3D/*.cpp Libraries/ImGui/*.cpp -I Simply3D -lassimp -lGLEW -lglfw -lGL -lX11 -lXrandr -lXi -lXinerama -lXcursor -std=c++17