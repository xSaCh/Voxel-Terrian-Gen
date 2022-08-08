#include "Chunk.h"
#include "util.h"
#include "../gfx/texture.h"

using namespace glm;
using namespace std;

void Chunk::setVoxels(bool (&data)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE])
{
	for (int z = 0; z < CHUNK_SIZE; z++)
		for (int y = 0 ; y < CHUNK_SIZE; y++)
			for (int x = 0 ; x < CHUNK_SIZE; x++)
			{
				voxels[x][y][z] = data[x][y][z];
			}
}

void Chunk::generateMesh()
{
	vector<Vertex> ve;
	vector<uint32_t> id;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (!voxels[x][y][z])
					continue;
				if (x == 0)
					updateVertex(ve, getFaceMesh(FaceSide::LEFT), vec3(x, y, z));
				if (y == 0)
					updateVertex(ve, getFaceMesh(FaceSide::DOWN), vec3(x, y, z));
				if (z == 0)
					updateVertex(ve, getFaceMesh(FaceSide::BACK), vec3(x, y, z));
				if (x == CHUNK_SIZE - 1)
					updateVertex(ve, getFaceMesh(FaceSide::RIGHT), vec3(x, y, z));
				if (y == CHUNK_SIZE - 1)
					updateVertex(ve, getFaceMesh(FaceSide::UP), vec3(x, y, z));
				if (z == CHUNK_SIZE - 1)
					updateVertex(ve, getFaceMesh(FaceSide::FRONT), vec3(x, y, z));

				if (x > 0 && !voxels[x - 1][y][z])
					updateVertex(ve, getFaceMesh(FaceSide::LEFT), vec3(x, y, z));
				if (y > 0 && voxels[x][y - 1][z] == false)
					updateVertex(ve, getFaceMesh(FaceSide::DOWN), vec3(x, y, z));
				if (z > 0 && voxels[x][y][z - 1] == false)
					updateVertex(ve, getFaceMesh(FaceSide::BACK), vec3(x, y, z));
				if (x + 1 < CHUNK_SIZE && voxels[x + 1][y][z] == false)
					updateVertex(ve, getFaceMesh(FaceSide::RIGHT), vec3(x, y, z));
				if (y + 1 < CHUNK_SIZE && voxels[x][y + 1][z] == false)
					updateVertex(ve, getFaceMesh(FaceSide::UP), vec3(x, y, z));
				if (z + 1 < CHUNK_SIZE && voxels[x][y][z + 1] == false)
					updateVertex(ve, getFaceMesh(FaceSide::FRONT), vec3(x, y, z));
			}

	Texture tex("res\\brick.png");
	vector<TextureInfo> t;
	TextureInfo ti;
	ti.id = tex.ID;
	ti.type = "texture_diffuse";
	t.push_back(ti);
	mesh.init(ve, id, t);
}