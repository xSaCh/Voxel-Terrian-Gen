#include "vbo.h"

VBO::VBO(void *data, GLsizeiptr size, bool isStaticDraw)
{
    glGenBuffers(1, &ID);
    bind();
    // glBufferData(GL_ARRAY_BUFFER, size, data, isStaticDraw ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
    glBufferData(GL_ARRAY_BUFFER, size, data, isStaticDraw ? GL_DYNAMIC_DRAW : GL_DYNAMIC_DRAW);
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