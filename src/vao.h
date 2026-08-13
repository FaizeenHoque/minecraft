#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "vbo.h"

class VAO
{
public:
    GLuint ID;
    VAO();

    void LinkVBO(VBO &VBO, GLuint layout);

    // Binds the VAO
    void Bind();
    // Unbinds the VAO
    void Unbind();
    // Deletes the VAO
    void Delete();
};
#endif