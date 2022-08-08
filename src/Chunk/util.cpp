#include "util.h"

vector<float> getFaceMesh(FaceSide side)
{
	vector<float> vert;
	int fact = 0;
	switch (side)
	{
	case FaceSide::BACK:
		fact = 0;
		break;
	case FaceSide::FRONT:
		fact = 1;
		break;
	case FaceSide::LEFT:
		fact = 2;
		break;
	case FaceSide::RIGHT:
		fact = 3;
		break;
	case FaceSide::DOWN:
		fact = 4;
		break;
	case FaceSide::UP:
		fact = 5;
		break;
	default:
		break;
	}
	for (int i = 0; i < 6 * 8; i++)
		vert.push_back(blockVertices[fact * (6 * 8) + i]);

	return vert;
}

void updateVertex(std::vector<Vertex> &data, std::vector<float> vert, glm::vec3 pos)
{
	for (int i = 0; i < 8 * 6; i += 8)
	{
		Vertex v;
		v.Position = glm::vec3(vert[i + 0] + pos.x, vert[i + 1] + pos.y, vert[i + 2] + pos.z);
		v.Normal = glm::vec3(vert[i + 3], vert[i + 4], vert[i + 5]);
		v.TexCoords = glm::vec2(vert[i + 6], vert[i + 7]);

		data.push_back(v);
	}
}