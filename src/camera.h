#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#define GLM_ENABLE_EXPERIMENTAL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "shaders.h"

class Camera
{
public:
    glm::vec3 Position;
    glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

    bool firstMouseSample = true;
    bool cursorLocked = false;

    int width, height;

    float walkSpeed = 4.5f;
    float sprintSpeed = 7.5f;
    float sensitivity = 0.12f;
    float lastFrameTime = 0.0f;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;

    Camera(int width, int height, glm::vec3 position);

    void Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform);
    void Inputs(GLFWwindow *window);
};

#endif
