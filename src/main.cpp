#include <iostream>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "perlin_noise.hpp"

#include "block.h"
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

GLfloat vertices[] = {
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

GLuint indices[] = {
    // Front
    0, 1, 2, 2, 3, 0,

    // Back
    4, 5, 6, 6, 7, 4,

    // Right
    8, 9, 10, 10, 11, 8,

    // Left
    12, 13, 14, 14, 15, 12,

    // Top
    16, 17, 18, 18, 19, 16,

    // Bottom
    20, 21, 22, 22, 23, 20};

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

    int actualWindowWidth, actualWindowHeight;
    glfwGetWindowSize(window, &actualWindowWidth, &actualWindowHeight);
    std::cout << "Window client size: " << actualWindowWidth << "x" << actualWindowHeight << std::endl;

    gladLoadGL();

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);

    Shader shaderProgram("src/shaders/default.vert", "src/shaders/default.frag");

    GLuint modelLoc = glGetUniformLocation(shaderProgram.ID, "model");
    GLuint tintLoc = glGetUniformLocation(shaderProgram.ID, "tint");

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

    Texture GrassBlockTexture_top("assets/textures/block/grass_block_top.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    GrassBlockTexture_top.texUnit(shaderProgram, "tex0", 0);

    Texture GrassBlockTexture_side("assets/textures/block/grass_block_side.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    GrassBlockTexture_side.texUnit(shaderProgram, "tex0", 0);

    Texture GrassBlockTexture_bottom("assets/textures/block/dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    GrassBlockTexture_bottom.texUnit(shaderProgram, "tex0", 0);

    Texture DirtBlockTexture_side("assets/textures/block/dirt.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    DirtBlockTexture_side.texUnit(shaderProgram, "tex0", 0);

    Texture StoneBlockTexture_side("assets/textures/block/stone.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    StoneBlockTexture_side.texUnit(shaderProgram, "tex0", 0);

    Texture BedrockBlockTexture_side("assets/textures/block/bedrock.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGBA, GL_UNSIGNED_BYTE);
    BedrockBlockTexture_side.texUnit(shaderProgram, "tex0", 0);

    BlockTexture GrassBlock_Textures = {
        &GrassBlockTexture_top,
        &GrassBlockTexture_bottom,
        &GrassBlockTexture_side,
        &GrassBlockTexture_side,
        &GrassBlockTexture_side,
        &GrassBlockTexture_side};
    BlockTexture DirtBlock_Textures = {
        &DirtBlockTexture_side,
        &DirtBlockTexture_side,
        &DirtBlockTexture_side,
        &DirtBlockTexture_side,
        &DirtBlockTexture_side,
        &DirtBlockTexture_side};
    BlockTexture StoneBlock_Textures = {
        &StoneBlockTexture_side,
        &StoneBlockTexture_side,
        &StoneBlockTexture_side,
        &StoneBlockTexture_side,
        &StoneBlockTexture_side,
        &StoneBlockTexture_side};
    BlockTexture BedrockBlock_Textures = {
        &BedrockBlockTexture_side,
        &BedrockBlockTexture_side,
        &BedrockBlockTexture_side,
        &BedrockBlockTexture_side,
        &BedrockBlockTexture_side,
        &BedrockBlockTexture_side,
    };

    Block GrassBlock = {BlockType::Grass, GrassBlock_Textures};
    Block DirtBlock = {BlockType::Dirt, DirtBlock_Textures};
    Block StoneBlock = {BlockType::Stone, StoneBlock_Textures};
    Block BedrockBlock = {BlockType::Bedrock, BedrockBlock_Textures};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

    Camera camera(framebufferWidth, framebufferHeight, glm::vec3(10.0f, 50.0f, 10.0f));

    bool wireframe = false;
    bool wireframeTogglePressed = false;

    World world;
    world.generate();

    for (Chunk &chunk : world.chunks)
    {
        for (Block &block : chunk.blocks)
        {
            switch (block.type)
            {
            case BlockType::Grass:
                block.textures = GrassBlock_Textures;
                break;

            case BlockType::Dirt:
                block.textures = DirtBlock_Textures;
                break;

            case BlockType::Stone:
                block.textures = StoneBlock_Textures;
                break;

            case BlockType::Bedrock:
                block.textures = BedrockBlock_Textures;
                break;
            }
        }
    }

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
        camera.Matrix(45.0f, 0.01f, 500.0f, shaderProgram, "camMatrix");

        VAO1.Bind();

        for (const Chunk &chunk : world.chunks)
        {
            for (const Block &block : chunk.blocks)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(block.position.x, block.position.y, block.position.z));

                glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

                // Front (+Z)
                if (world.isAir(block.position.x, block.position.y, block.position.z + 1))
                {
                    block.textures.front->Bind();
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)0);
                }

                // Back (-Z)
                if (world.isAir(block.position.x, block.position.y, block.position.z - 1))
                {
                    block.textures.back->Bind();
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(6 * sizeof(GLuint)));
                }

                // Right (+X)
                if (world.isAir(block.position.x + 1, block.position.y, block.position.z))
                {
                    block.textures.right->Bind();
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(12 * sizeof(GLuint)));
                }

                // Left (-X)
                if (world.isAir(block.position.x - 1, block.position.y, block.position.z))
                {
                    block.textures.left->Bind();
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(18 * sizeof(GLuint)));
                }

                // Top (+Y)
                if (world.isAir(block.position.x, block.position.y + 1, block.position.z))
                {
                    block.textures.top->Bind();

                    if (block.type == BlockType::Grass)
                        glUniform3f(tintLoc, 0.7f, 1.0f, 0.6f);
                    else
                        glUniform3f(tintLoc, 1.0f, 1.0f, 1.0f);

                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(24 * sizeof(GLuint)));

                    glUniform3f(tintLoc, 1.0f, 1.0f, 1.0f);
                }

                // Bottom (-Y)
                if (world.isAir(block.position.x, block.position.y - 1, block.position.z))
                {
                    block.textures.bottom->Bind();
                    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void *)(30 * sizeof(GLuint)));
                }
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    VAO1.Delete();
    VBO1.Delete();
    EBO1.Delete();
    shaderProgram.Delete();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}