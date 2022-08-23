#ifndef CHUNK_H
#define CHUNK_H

#include <iostream>
#include <vector>
#include "chunkMesh.h"
#include "../gfx/shader.h"

#define CHUNK_SIZE 32

class Chunk
{
public:
    Chunk() {}
    ~Chunk()
    { /*std::cout << "Chunk Decons.\n";*/
    }
    void generateMesh();
    void setVoxels(bool (&data)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]);
    void updateBlock(int x, int y, int z, bool val);
    void draw(Shader &shader) { mesh.draw(shader); }

    bool voxels[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = {{{false}}};

private:
    ChunkMesh mesh;
};

#endif