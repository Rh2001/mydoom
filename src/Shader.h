// Shader.h header file to load shaders from a file

#pragma once
#include <string>
#include <glad/gl.h>
class Shader {
public:
    unsigned int ID; // Id of the shader program
    Shader(const char* vertexPath, const char* fragmentPath); // Constructor to load and compile shaders from file paths
    void use();
};