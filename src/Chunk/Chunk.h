#ifndef CHUNK_H
#define CHUNK_H

#include <vector>
#include "../gfx/mesh.h"
#include "../gfx/shader.h"

#define CHUNK_SIZE 32

class Chunk
{
public:
    Chunk() {}
    ~Chunk() {}
    void generateMesh();
    void setVoxels(bool (&data)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
    void draw(Shader &shader){mesh.draw(shader);}
private:
    bool voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = {{{false}}};
    Mesh mesh;
};

#endif