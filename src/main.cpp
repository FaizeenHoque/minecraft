#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "texture.h"
#include "shaders.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "stb_image.h"
#include "camera.h"

const unsigned int width = 1600;
const unsigned int height = 800;

GLfloat vertices[] =
    {
        // Position              Color              TexCoord

        // Base
        -0.5f, 0.0f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 0
        0.5f, 0.0f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 1
        0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,   // 2
        -0.5f, 0.0f, 0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 3

        // Apex
        0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.5f, 0.5f // 4
};

GLuint indices[] =
    {
        // Base
        0, 1, 2,
        0, 2, 3,

        // Front
        0, 1, 4,

        // Right
        1, 2, 4,

        // Back
        2, 3, 4,

        // Left
        3, 0, 4};

int main()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow *window = glfwCreateWindow(width, height, "Minecraft", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glfwSetWindowSizeLimits(window, width, height, width, height);
    glfwSetWindowSize(window, width, height);

    int actualWindowWidth;
    int actualWindowHeight;
    glfwGetWindowSize(window, &actualWindowWidth, &actualWindowHeight);
    std::cout << "Window client size: " << actualWindowWidth << "x" << actualWindowHeight << std::endl;

    gladLoadGL();

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    Shader shaderProgram(
        "src/shaders/default.vert",
        "src/shaders/default.frag");

    VAO VAO1;
    VAO1.Bind();

    VBO VBO1(vertices, sizeof(vertices));
    EBO EBO1(indices, sizeof(indices));

    VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void *)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void *)(6 * sizeof(float)));

    VAO1.Unbind();
    VBO1.Unbind();
    EBO1.Unbind();

    Texture dirt("assets/dirt.jpeg", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    dirt.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);

    Camera camera(framebufferWidth, framebufferHeight, glm::vec3(0.0f, 0.0f, 3.0f));

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.01f, 100.0f, shaderProgram, "camMatrix");

        dirt.Bind();
        VAO1.Bind();
        glDrawElements(GL_TRIANGLES, sizeof(indices) / sizeof(int), GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    dirt.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
