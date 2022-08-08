#ifndef VBO_H
#define VBO_H
#include <glad/glad.h>
#include <cstdint>

// Vertex Buffer object Class
class VBO
{
public:
    uint32_t ID;
    VBO(void *data, GLsizeiptr size);
    void bind();
    void unbind();
    void deleteVBO();
};

#endif
