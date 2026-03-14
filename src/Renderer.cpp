// Renderer.cpp - handles all rendering logic for the raycasting engine, pretty bare bones rn.

#include "Renderer.h"
#include <glad/gl.h>
#include <cmath>
#include <algorithm>
#include <iostream>
#include "Shader.h"

extern Shader* shader; // Global shader pointer from main.cpp

Renderer::Renderer(int width, int height) {
    screenWidth = width;
    screenHeight = height;

    // Setup VAO/VBO/EBO for a single quad, updated per slice
    float vertices[24] = {0}; // 4 vertices * 6 floats (pos + color)
    unsigned int indices[] = {0,1,2, 2,3,0};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

Renderer::~Renderer() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}

void Renderer::renderFrame(float playerX, float playerY, float playerAngle, Map& map) {
    float fov = 60.0f * 3.14159f / 180.0f;

    for (int x = 0; x < screenWidth; ++x) {
        float rayAngle = (playerAngle - fov/2.0f) + fov * ((float)x / screenWidth);

        // Raycasting
        float rayX = playerX, rayY = playerY;
        float rayStep = 0.05f;
        float distance = 0.0f;

        while (true) {
            rayX += cos(rayAngle) * rayStep;
            rayY += sin(rayAngle) * rayStep;
            distance += rayStep;

            int mapX = (int)rayX;
            int mapY = (int)rayY;
            if (map.isWall(mapX, mapY)) break;
        }

        if (distance < 0.1f) distance = 0.1f;   // avoid too close
        if (distance > 20.0f) distance = 20.0f; // avoid huge far walls

        // apply simple fisheye correction so edges aren't over‑stretched
        float corrected = distance * cosf(rayAngle - playerAngle);
        if (corrected < 0.1f) corrected = 0.1f;

        int lineHeight = (int)(screenHeight / corrected);
        lineHeight = std::clamp(lineHeight, 1, screenHeight * 3 / 4);

        float ndcHeight = lineHeight / (float)screenHeight * 2.0f; // scale to NDC [-1,1]
        float yTop = ndcHeight / 2.0f;
        float yBottom = -ndcHeight / 2.0f;

        float sliceWidth = 2.0f / screenWidth;
        float sliceX = (x / (float)screenWidth) * 2.0f - 1.0f;

        float brightness = 1.0f / (corrected * corrected); // quadratic falloff based on corrected distance
        brightness = std::clamp(brightness, 0.1f, 1.0f);

        float vertices[24] = {
            sliceX, yBottom, 0.0f, brightness, brightness, brightness,
            sliceX+sliceWidth, yBottom, 0.0f, brightness, brightness, brightness,
            sliceX+sliceWidth, yTop, 0.0f, brightness, brightness, brightness,
            sliceX, yTop, 0.0f, brightness, brightness, brightness,
        };

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        shader->use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
}