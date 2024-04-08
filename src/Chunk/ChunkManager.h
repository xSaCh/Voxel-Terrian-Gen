#ifndef CHUNK_MAN_H
#define CHUNK_MAN_H

#include <unordered_map>
#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <PerlinNoise.hpp>

#include "Chunk.h"

using namespace std;
using namespace glm;
class ChunkManager
{
public:
    ChunkManager()
    {
        loadRadius = 4;
        prevLoadPos = vec2(0.0f);
    }
    ~ChunkManager();

    void updateChunks(vec3 &pos, bool forceUpdate = false);
    Chunk *getChunkAt(vec3 &pos);

    unordered_map<vec3, Chunk *> loadedC;
    int loadRadius;
    vec2 prevLoadPos;

private:
    Chunk *genChunk(vec3 worldPos, int i);

    siv::PerlinNoise::seed_type seed = 123456u;
    siv::PerlinNoise perlin{seed};
    float noiseFactor = 0.027f;
};

#endif
