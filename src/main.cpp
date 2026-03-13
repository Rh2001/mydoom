#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Shader.h"

const int WIDTH = 1280;
const int HEIGHT = 720;

int main()
{

    float vertices[] = {
        -0.5f,-0.5f,0.0f,  0.5f,-0.5f,0.0f,
         0.5f, 0.5f,0.0f, -0.5f, 0.5f,0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        2,3,0
    };

    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyDoom", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!window)
    {
        std::cout << "Failed to create window... terminating with error:\n" + std::to_string(glfwGetError(NULL)) + "\n";
        return -1;
    }

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "GLAD failed\n";
        return -1;
    }

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl"); // Load and compile shaders (after GL is loaded)

    unsigned int VAO,VBO,EBO;
    glGenVertexArrays(1,&VAO);
    glGenBuffers(1,&VBO);
    glGenBuffers(1,&EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(indices),indices,GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),(void*)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f,0.1f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}