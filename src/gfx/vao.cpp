#include "vao.h"

VAO::VAO()
{
    glad_glGenVertexArrays(1, &ID);
}

void VAO::linkVBO(VBO &vbo, uint32_t layout, int size, GLenum type, bool normalized, GLsizei stride, uint32_t offset)
{
    vbo.bind();

    //(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void *pointer)
    glVertexAttribPointer(layout, size, type, normalized, stride, (void *)offset);
    glEnableVertexAttribArray(layout);

    vbo.unbind();
}

void VAO::bind()
{
    glBindVertexArray(ID);
}
void VAO::unbind()
{
    glBindVertexArray(0);
}

void VAO::deleteVAO()
{
    glDeleteVertexArrays(1, &ID);
}