#ifndef VAO_CLASS_H
#define VAO_CLASS_H

#include <glad/glad.h>
#include "vbo.h"

class VAO
{
public:
    GLuint ID;
    VAO();

    void LinkAttrib(VBO &VBO, GLuint layout, GLenum numComponents, GLenum type, GLsizeiptr stride, void *offset);

    // Binds the VAO
    void Bind();
    // Unbinds the VAO
    void Unbind();
    // Deletes the VAO
    void Delete();
};
#endif