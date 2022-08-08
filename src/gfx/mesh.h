#ifndef MESH_H
#define MESH_H

#include <iostream>
#include <vector>
#include <glm\glm.hpp>

#include "shader.h"
#include "vao.h"
#include "vbo.h"
#include "ebo.h"

using namespace std;

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct TextureInfo
{
    uint32_t id;
    string type;
    string path;
};

class Mesh
{
public:
    Mesh(vector<Vertex> vertices, vector<uint32_t> indices, vector<TextureInfo> textures);
    ~Mesh();
    void draw(Shader &shader);

    vector<Vertex> vertices;
    vector<uint32_t> indices;
    vector<TextureInfo> textures;

    VAO *vao = NULL;
    VBO *vbo = NULL;
    EBO *ebo = NULL;

private:
    void setupMesh();
    bool hasIndices;
};

#endif