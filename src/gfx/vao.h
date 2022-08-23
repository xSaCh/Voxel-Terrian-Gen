#ifndef VAO_H
#define VAO_H

#include "vbo.h"
#include <glad/glad.h>
#include <cstdint>

// Vertex Array object Class
class VAO
{
public:
    uint32_t ID;
    VAO();

    void linkVBO(VBO &vbo, uint32_t layout, int size, GLenum type, bool normalized, GLsizei stride, uint32_t offset);
    void bind();
    void unbind();
    void deleteVAO();
};

#endif
