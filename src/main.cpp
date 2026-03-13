#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>

const int WIDTH = 1280;
const int HEIGHT = 720;

int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "MyDoom", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress))
    {
        std::cout << "GLAD failed\n";
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.1f,0.1f,0.2f,1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}