#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "perlin_noise.hpp"

#include "texture.h"
#include "shaders.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"
#include "stb_image.h"
#include "camera.h"
#include "world.h"

const unsigned int width = 1600;
const unsigned int height = 800;

GLfloat vertices[] =
    {
        // Position              Color              TexCoord

        // Front
        -0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 0,
        0.5f, -0.5f, 0.5f, 0, 1, 0, 1, 0,
        0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1,
        -0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1,

        // Back
        0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 0,
        -0.5f, -0.5f, -0.5f, 0, 1, 0, 1, 0,
        -0.5f, 0.5f, -0.5f, 0, 0, 1, 1, 1,
        0.5f, 0.5f, -0.5f, 1, 1, 0, 0, 1,

        // Right
        0.5f, -0.5f, 0.5f, 1, 0, 0, 0, 0,
        0.5f, -0.5f, -0.5f, 0, 1, 0, 1, 0,
        0.5f, 0.5f, -0.5f, 0, 0, 1, 1, 1,
        0.5f, 0.5f, 0.5f, 1, 1, 0, 0, 1,

        // Left
        -0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 0,
        -0.5f, -0.5f, 0.5f, 0, 1, 0, 1, 0,
        -0.5f, 0.5f, 0.5f, 0, 0, 1, 1, 1,
        -0.5f, 0.5f, -0.5f, 1, 1, 0, 0, 1,

        // Top
        -0.5f, 0.5f, 0.5f, 1, 0, 0, 0, 0,
        0.5f, 0.5f, 0.5f, 0, 1, 0, 1, 0,
        0.5f, 0.5f, -0.5f, 0, 0, 1, 1, 1,
        -0.5f, 0.5f, -0.5f, 1, 1, 0, 0, 1,

        // Bottom
        -0.5f, -0.5f, -0.5f, 1, 0, 0, 0, 0,
        0.5f, -0.5f, -0.5f, 0, 1, 0, 1, 0,
        0.5f, -0.5f, 0.5f, 0, 0, 1, 1, 1,
        -0.5f, -0.5f, 0.5f, 1, 1, 0, 0, 1};

GLuint indices[] =
    {
        // Front
        0, 1, 2,
        2, 3, 0,

        // Back
        4, 5, 6,
        6, 7, 4,

        // Right
        8, 9, 10,
        10, 11, 8,

        // Left
        12, 13, 14,
        14, 15, 12,

        // Top
        16, 17, 18,
        18, 19, 16,

        // Bottom
        20, 21, 22,
        22, 23, 20};

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

    GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");

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

    Texture dirt("assets/dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    dirt.texUnit(shaderProgram, "tex0", 0);
    Texture stone("assets/stone.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    stone.texUnit(shaderProgram, "tex0", 0);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Camera camera(framebufferWidth, framebufferHeight, glm::vec3(0.0f, 0.0f, 3.0f));
    bool wireframe = false;
    bool wireframeTogglePressed = false;

    while (!glfwWindowShouldClose(window))
    {
        const bool togglePressed = glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS;
        if (togglePressed && !wireframeTogglePressed)
        {
            wireframe = !wireframe;
            glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        }
        wireframeTogglePressed = togglePressed;

        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaderProgram.Activate();

        camera.Inputs(window);
        camera.Matrix(45.0f, 0.01f, 100.0f, shaderProgram, "camMatrix");

        VAO1.Bind();

        World world;
        world.generate();

        for (auto &block : world.blocks)
        {
            if (block.type == BlockType::Dirt)
            {
                dirt.Bind();
            }
            else if (block.type == BlockType::Stone)
            {
                stone.Bind();
            }

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, block.position);

            glUniformMatrix4fv(
                modelLoc,
                1,
                GL_FALSE,
                glm::value_ptr(model));

            glDrawElements(
                GL_TRIANGLES,
                sizeof(indices) / sizeof(indices[0]),
                GL_UNSIGNED_INT,
                0);
        }

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
