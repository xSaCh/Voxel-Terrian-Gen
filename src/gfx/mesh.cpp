#include <string>

#include "mesh.h"

Mesh::Mesh(vector<Vertex> vertices, vector<uint32_t> indices, vector<TextureInfo> textures)
{
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    hasIndices = indices.size() > 1 ? true : false;
    setupMesh();
}

void Mesh::draw(Shader &shader)
{
    uint32_t diffuseNr = 1;
    uint32_t specularNr = 1;
    for (uint32_t i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        // retrieve texture number (the N in diffuse_textureN)
        string number;
        string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    // glActiveTexture(GL_TEXTURE0);

    // draw mesh
    vao->bind();
    // vbo->bind();
    // ebo->bind();
    if (hasIndices)
        // glDrawElements(GL_LINES, indices.size(), GL_UNSIGNED_INT, 0);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    else
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        // glDrawArrays(GL_LINES, 0, vertices.size());
    vao->unbind();
}

void Mesh::setupMesh()
{
    vao = new VAO();
    vao->bind();
    vbo = new VBO(&(this->vertices[0]), this->vertices.size() * sizeof(Vertex));
    ebo = new EBO(&(this->indices[0]), this->indices.size() * sizeof(uint32_t));
    vao->linkVBO(*vbo, 0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    vao->linkVBO(*vbo, 1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, Normal));
    vao->linkVBO(*vbo, 2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), offsetof(Vertex, TexCoords));

    vao->unbind();

    // printf("[Debug] vao: %i %p vbo: %i %p ebo: %i %p\n", vao->ID, (void *)vao, vbo->ID, (void *)vbo, ebo->ID, (void *)ebo);
}

Mesh::~Mesh()
{
    ebo->deleteEBO();
    vbo->deleteVBO();
    vao->deleteVAO();

    delete ebo;
    delete vbo;
    delete vao;
}