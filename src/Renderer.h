#pragma once
#include "Map.h"

class Renderer {
private:
    int screenWidth;
    int screenHeight;
    
    unsigned int VAO, VBO, EBO;

public:
    Renderer(int width, int height);
    ~Renderer();
    void renderFrame(float playerX, float playerY, float playerAngle, Map& map);
};