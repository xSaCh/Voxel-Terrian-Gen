#include "vbo.h"

VBO::VBO(void *data, GLsizeiptr size)
{
    glGenBuffers(1, &ID);
    bind();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VBO::bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, ID);
}
void VBO::unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::deleteVBO()
{
    glDeleteBuffers(1, &ID);
}