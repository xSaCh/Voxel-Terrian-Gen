#ifndef CHUNK_MESH_H
#define CHUNK_MESH_H

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "../gfx/mesh.h"
#include "../gfx/shader.h"
#include "../gfx/vao.h"
#include "../gfx/vbo.h"
#include "../gfx/ebo.h"

using namespace std;

class ChunkMesh
{
public:
    ChunkMesh() {}
    ChunkMesh(vector<Vertex> vertices, vector<TextureInfo> textures);
    ~ChunkMesh();

    void init(vector<Vertex> vertices, vector<TextureInfo> textures);
    void update(vector<Vertex> vertices);
    void draw(Shader &shader);
    vector<Vertex> vertices;
    vector<TextureInfo> textures;

    VAO *vao = NULL;
    VBO *vbo = NULL;

    bool isInit = false;
    // EBO *ebo = NULL;

private:
    void setupMesh();
};

#endif