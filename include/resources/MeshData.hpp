#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <vector>

struct MeshData
{
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> tangents;
	std::vector<glm::vec2> textureCoords;
	std::vector<unsigned int> indices;

	void addData(const MeshData& otherData, const glm::mat4& offset);
	void addData(const MeshData& otherData, const std::vector<glm::mat4>& offsets);
};

const float pi = 3.1415926536f;

MeshData createTriangle(float size = 1.0f);
MeshData createSphere(float radius = 0.5f, int stacks = 8, int slices = 12);
