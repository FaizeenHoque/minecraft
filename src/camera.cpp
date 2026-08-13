#include "camera.h"

Camera::Camera(int width, int height, glm::vec3 position)
{
    Camera::width = width;
    Camera::height = height;
    Position = position;
}

void Camera::Matrix(float FOVdeg, float nearPlane, float farPlane, Shader &shader, const char *uniform)
{
    // Initializes matrices since otherwise they will be the null matrix
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    // Makes camera look in the right direction from the right position
    view = glm::lookAt(Position, Position + Orientation, Up);
    // Adds perspective to the scene
    projection = glm::perspective(glm::radians(FOVdeg), (float)width / height, nearPlane, farPlane);

    // Exports the camera matrix to the Vertex Shader
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, uniform), 1, GL_FALSE, glm::value_ptr(projection * view));
}

void Camera::Inputs(GLFWwindow *window)
{
    // GLFW_CURSOR_DISABLED captures the pointer and supplies relative-style
    // motion, so looking around never lets the cursor reach a window edge.
    if (!cursorLocked)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        if (glfwRawMouseMotionSupported())
            glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
        cursorLocked = true;
        firstMouseSample = true;
    }

    const float now = static_cast<float>(glfwGetTime());
    const float deltaTime = lastFrameTime == 0.0f ? 0.0f : now - lastFrameTime;
    lastFrameTime = now;
    const float moveSpeed = (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
                                ? sprintSpeed
                                : walkSpeed;
    const float distance = moveSpeed * deltaTime;

    // Minecraft movement stays level even while the player is looking up/down.
    glm::vec3 forward = glm::normalize(glm::vec3(Orientation.x, 0.0f, Orientation.z));
    glm::vec3 right = glm::normalize(glm::cross(forward, Up));

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        Position += distance * forward;
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        Position -= distance * right;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        Position -= distance * forward;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        Position += distance * right;
    }
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    {
        Position += distance * Up;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    {
        Position -= distance * Up;
    }

    double mouseX;
    double mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Ignore the initial position reported when cursor capture begins.
    if (firstMouseSample)
    {
        lastMouseX = mouseX;
        lastMouseY = mouseY;
        firstMouseSample = false;
        return;
    }

    const float yaw = static_cast<float>(mouseX - lastMouseX) * sensitivity;
    const float pitch = static_cast<float>(lastMouseY - mouseY) * sensitivity;
    lastMouseX = mouseX;
    lastMouseY = mouseY;

    glm::vec3 pitched = glm::rotate(Orientation, glm::radians(pitch), right);
    // Leave a small margin from vertical to prevent the camera from flipping.
    if (abs(glm::angle(pitched, Up) - glm::radians(90.0f)) <= glm::radians(89.0f))
        Orientation = pitched;

    Orientation = glm::normalize(glm::rotate(Orientation, glm::radians(-yaw), Up));
}
