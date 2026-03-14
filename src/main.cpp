#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"
#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h" // Music to make myself feel good that it is a game not random vertices
#include "SDL2/SDL_mixer.h"
#include "Renderer.h"
#include "Map.h"
#include <chrono>
#include <thread>


const int WIDTH = 1280;
const int HEIGHT = 720;
// 30 FPS cap, you can raise it, the game has no other logic or physics rn. Simulates original DOOM's 30 fps cap.
const double FRAME_TIME = 1.0 / 30.0;

// Player state and initial location
struct Player {
    float x = 5.5f; // Start in open space, not inside a wall, this took me 20 decades to debug, it is a hardcoded value based on map.cpp
    float y = 5.5f;
    float angle = 0.0f; 
    float moveSpeed = 0.05f;
    float rotSpeed = 0.03f;
};

// Global instances
Shader* shader;
Renderer* renderer;
Map map;
Player player;
GLFWwindow* window = nullptr; // Initial nullptr for safety
Mix_Music* bgm = nullptr;

// Functions to intialize SDL, OpenGL, and our Renderer/Shader
bool initSDL() {
    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cout << "SDL2 init failed: " << SDL_GetError() << "\n";
        return false;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "SDL2_mixer init failed: " << Mix_GetError() << "\n";
        return false;
    }

    bgm = Mix_LoadMUS("./assets/music/d_runnin.ogg");
    if (!bgm) {
        std::cout << "Failed to load music: " << Mix_GetError() << "\n";
    } else {
        Mix_PlayMusic(bgm, -1);
    }
    return true;
}

bool initGLFW() {
    if (!glfwInit()) return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(WIDTH, HEIGHT, "MyDoom", nullptr, nullptr);
    if (!window) return false;
    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) return false;

    glViewport(0, 0, WIDTH, HEIGHT);
    return true;
}

bool initRendererAndShader() {
    shader = new Shader("./shaders/vertex.glsl", "./shaders/fragment.glsl");
    renderer = new Renderer(WIDTH, HEIGHT);
    return true;
}

// Input handling
void processInput() {
    if (!window) return;

    if (glfwGetKey(window, GLFW_KEY_LEFT)) player.angle -= player.rotSpeed;
    if (glfwGetKey(window, GLFW_KEY_RIGHT)) player.angle += player.rotSpeed;

    if (glfwGetKey(window, GLFW_KEY_UP)) {
        float nx = player.x + cos(player.angle) * player.moveSpeed;
        float ny = player.y + sin(player.angle) * player.moveSpeed;
        if (!map.isWall((int)nx, (int)player.y)) player.x = nx;
        if (!map.isWall((int)player.x, (int)ny)) player.y = ny;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN)) {
        float nx = player.x - cos(player.angle) * player.moveSpeed;
        float ny = player.y - sin(player.angle) * player.moveSpeed;
        if (!map.isWall((int)nx, (int)player.y)) player.x = nx;
        if (!map.isWall((int)player.x, (int)ny)) player.y = ny;
    }
}

// Render 
void renderFrame() {
    glClearColor(0.1f, 0.1f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    renderer->renderFrame(player.x, player.y, player.angle, map);
}

// Cleanup and free resources
void cleanup() {
    if (bgm) Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    SDL_Quit();

    delete shader;
    delete renderer;

    if (window) glfwTerminate();
}

// Main function
int main() {
    std::cout << "This is a Doom clone in C++ using OpenGL, GLFW, GLAD and uses SDL2 for music.\n";
    std::cout << "Use arrow keys to move and turn, game engine is pretty bare bones right now\n";
    std::cout << "I did not include the music file in the repo, you can add your own music file named 'd_runnin.ogg' in the assets/music folder to play it.\n";
    std::cout << "This is a work in progress hobby project, so expect bugs and crashes, but it is a fun learning experience for me.\n";
    std::cout << "Press Enter to start...\n";
    std::cin.get(); 



    if (!initSDL()) return -1;
    if (!initGLFW()) return -1;
    initRendererAndShader();

    while (!glfwWindowShouldClose(window)) { // Game loop is here
        auto frameStart = std::chrono::high_resolution_clock::now();
        processInput();
        renderFrame();
        glfwSwapBuffers(window);
        glfwPollEvents();
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed = frameEnd - frameStart;

        double sleepTime = FRAME_TIME - elapsed.count();
        if (sleepTime > 0.0) {

            std::this_thread::sleep_for(
                std::chrono::duration<double>(sleepTime)
            );
        }
    }

    cleanup();
    return 0;
}

void updateLoop() {
    //TODO
    
}