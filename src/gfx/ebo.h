#ifndef EBO_H
#define EBO_H
#include <glad/glad.h>
#include <cstdint>

// ELement Buffer object Class(indices)
class EBO
{
public:
    uint32_t ID;
    EBO(void *data, GLsizeiptr size);
    void bind();
    void unbind();
    void deleteEBO();
};

#endif
