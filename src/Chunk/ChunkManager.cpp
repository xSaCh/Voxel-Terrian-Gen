#include <iostream>
#include "ChunkManager.h"

ChunkManager::~ChunkManager()
{
    for (auto it = loadedC.cbegin(), next_it = it; it != loadedC.cend(); it = next_it)
    {
        ++next_it;
        delete loadedC.at(it->first);
        loadedC.erase(it);
    }
}

void ChunkManager::updateChunks(vec3 &pos, bool forceUpdate)
{
    vec2 p = vec2(floor(pos.x / 32), floor(pos.z / 32));
    p = (float)CHUNK_SIZE * p;

    int count = 0;
    if (forceUpdate)
    {
        prevLoadPos = p;

        // delete all chunks
        for (auto it = loadedC.cbegin(), next_it = it; it != loadedC.cend(); it = next_it)
        {
            ++next_it;
            delete loadedC.at(it->first);
            loadedC.erase(it);
        }

        for (int y = -loadRadius; y < loadRadius; y++)
            for (int x = -loadRadius; x < loadRadius; x++)
            {
                // vec3 p = vec3(prevLoadPos.x + (x * CHUNK_SIZE), 0.0f, prevLoadPos.y + (y * CHUNK_SIZE));
                vec3 newChunkPos = vec3(prevLoadPos.x + (x * CHUNK_SIZE), 0.0f, prevLoadPos.y + (y * CHUNK_SIZE));
                Chunk *c = genChunk(newChunkPos, x + (y * loadRadius));
                loadedC.emplace(newChunkPos, c);
                count++;
            }

        cout << "Force TotallLoad: " << count << '\n';
    }
    else if (prevLoadPos != p)
    {
        prevLoadPos = p;

        // delete all chunks that are away from player
        for (auto it = loadedC.cbegin(), next_it = it; it != loadedC.cend(); it = next_it)
        {
            ++next_it;
            if (abs(distance(pos, it->first)) > loadRadius * CHUNK_SIZE)
            {
                // printf("Remove Chunk: %f : %.2f %.2f %.2f \n", distance(pos, it->first), it->first.x, it->first.y, it->first.z);
                delete loadedC.at(it->first);
                loadedC.erase(it);
            }
        }

        for (int y = -loadRadius; y < loadRadius; y++)
            for (int x = -loadRadius; x < loadRadius; x++)
            {
                // vec3 p = vec3(prevLoadPos.x + (x * CHUNK_SIZE), 0.0f, prevLoadPos.y + (y * CHUNK_SIZE));
                vec3 newChunkPos = vec3(prevLoadPos.x + (x * CHUNK_SIZE), 0.0f, prevLoadPos.y + (y * CHUNK_SIZE));
                if (!loadedC.count(newChunkPos))
                {
                    Chunk *c = genChunk(newChunkPos, x + (y * loadRadius));
                    loadedC.emplace(newChunkPos, c);
                    count++;
                }
            }

        cout << "TotallLoad: " << count << '\n';
    }
}

Chunk *ChunkManager::getChunkAt(vec3 &pos) { return loadedC.at(pos); }

Chunk *ChunkManager::genChunk(vec3 worldPos, int i)
{
    Chunk *chunkM = new Chunk();
    bool chunk[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE] = {{{false}}};

    if (worldPos != vec3(0.0f))
        for (int x = 0; x < CHUNK_SIZE; x++)
            for (int z = 0; z < CHUNK_SIZE; z++)
            {
                double n = perlin.octave2D_01((x + worldPos.x) * noiseFactor, (z + worldPos.z) * noiseFactor, 8);

                int height = n * (CHUNK_SIZE - 1);
                for (int y = 0; y < height; y++)
                    chunk[x][y][z] = true;
            }
    chunkM->setVoxels(chunk);
    chunkM->generateMesh();

    return chunkM;
}