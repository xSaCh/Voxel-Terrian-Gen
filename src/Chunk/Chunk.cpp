#include "Chunk.h"
#include "util.h"
#include "../gfx/texture.h"
#include <cstring>

using namespace glm;
using namespace std;

void Chunk::setVoxels(bool (&data)[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE])
{
	for (int z = 0; z < CHUNK_SIZE; z++)
		for (int y = 0; y < CHUNK_SIZE; y++)
			for (int x = 0; x < CHUNK_SIZE; x++)
			{
				voxels[x][y][z] = data[x][y][z];
			}
}
void Chunk::updateBlock(int x, int y, int z, bool val)
{
	// assert(x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE);
	// for (int x = 0; x < CHUNK_SIZE; x++)
	// 	for (int y = 0; y < CHUNK_SIZE; y++)
	// 		for (int z = 0; z < CHUNK_SIZE; z++)
	// 		{
	voxels[x][y][z] = val;
	// }
	// generateMesh();
}

void Chunk::generateMesh()
{
	vector<Vertex> ve;

	for (int x = 0; x < CHUNK_SIZE; x++)
		for (int y = 0; y < CHUNK_SIZE; y++)
			for (int z = 0; z < CHUNK_SIZE; z++)
			{
				if (!voxels[x][y][z])
					continue;
				int bID = 2; // grass

				if (y <= 10)
					bID = 1; // Water
				else if (y > 13 && y <= 20)
					bID = 0; // dirt
				else if (y > 20)
					bID = 3; // snow

				if (x == 0)
					updateVertex(ve, bID, getFaceMesh(FaceSide::LEFT), vec3(x, y, z));
				if (y == 0)
					updateVertex(ve, bID, getFaceMesh(FaceSide::DOWN), vec3(x, y, z));
				if (z == 0)
					updateVertex(ve, bID, getFaceMesh(FaceSide::BACK), vec3(x, y, z));
				if (x == CHUNK_SIZE - 1)
					updateVertex(ve, bID, getFaceMesh(FaceSide::RIGHT), vec3(x, y, z));
				if (y == CHUNK_SIZE - 1)
					updateVertex(ve, bID, getFaceMesh(FaceSide::UP), vec3(x, y, z));
				if (z == CHUNK_SIZE - 1)
					updateVertex(ve, bID, getFaceMesh(FaceSide::FRONT), vec3(x, y, z));

				if (x > 0 && !voxels[x - 1][y][z])
					updateVertex(ve, bID, getFaceMesh(FaceSide::LEFT), vec3(x, y, z));
				if (y > 0 && voxels[x][y - 1][z] == false)
					updateVertex(ve, bID, getFaceMesh(FaceSide::DOWN), vec3(x, y, z));
				if (z > 0 && voxels[x][y][z - 1] == false)
					updateVertex(ve, bID, getFaceMesh(FaceSide::BACK), vec3(x, y, z));
				if (x + 1 < CHUNK_SIZE && voxels[x + 1][y][z] == false)
					updateVertex(ve, bID, getFaceMesh(FaceSide::RIGHT), vec3(x, y, z));
				if (y + 1 < CHUNK_SIZE && voxels[x][y + 1][z] == false)
					updateVertex(ve, bID, getFaceMesh(FaceSide::UP), vec3(x, y, z));
				if (z + 1 < CHUNK_SIZE && voxels[x][y][z + 1] == false)
					updateVertex(ve, bID, getFaceMesh(FaceSide::FRONT), vec3(x, y, z));
			}

	Texture tex("res/DemoAtlas.jpg");
	vector<TextureInfo> t;
	TextureInfo ti;
	ti.id = tex.ID;
	ti.type = "texture_diffuse";
	t.push_back(ti);

	if (!mesh.isInit)
		mesh.init(ve, t);
	else
	{
		printf("Update Mesh\n");

		// mesh.vao->bind();
		mesh.vbo->bind();
		// glBufferData(GL_ARRAY_BUFFER, ve.size() * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);
		glBufferData(GL_ARRAY_BUFFER, ve.size() * sizeof(Vertex), &(ve[0]), GL_DYNAMIC_DRAW);

		vector<Vertex> *d = (vector<Vertex> *)malloc(ve.size() * sizeof(Vertex));
		glGetBufferSubData(GL_ARRAY_BUFFER, 0, ve.size() * sizeof(Vertex), d);

		// glBufferSubData(GL_ARRAY_BUFFER, 0, ve.size() * sizeof(Vertex), &(ve[0]));
		// void *ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		// memcpy(ptr, &(ve[0]), ve.size() * sizeof(Vertex));
		// glUnmapBuffer(GL_ARRAY_BUFFER);
		// mesh.vao->unbind();
		mesh.vbo->unbind();
		// vbo = new VBO(&(this->vertices[0]), this->vertices.size() * sizeof(Vertex));
	}
}