#include "resources/MeshData.hpp"
#include "glm/gtx/normal.hpp"
#include <assert.h>

void MeshData::addData(const MeshData& otherData, const glm::mat4& offset)
{
	assert(static_cast<unsigned long int>(positions.size()) + static_cast<unsigned long int>(otherData.positions.size()) < static_cast<unsigned long int>(std::numeric_limits<unsigned int>::max()));

	indices.reserve(indices.size() + otherData.indices.size());
	positions.reserve(positions.size() + otherData.positions.size());
	normals.reserve(normals.size() + otherData.normals.size());
	tangents.reserve(tangents.size() + otherData.tangents.size());
	textureCoords.reserve(textureCoords.size() + otherData.textureCoords.size());

	for (auto index : otherData.indices) {
		indices.emplace_back(static_cast<unsigned int>(positions.size()) + index);
	}
	
	for (const auto& position : otherData.positions) {
		positions.emplace_back(offset * glm::vec4(position, 1.0f));
	}
	
	for (const auto& normal : otherData.normals) {
		normals.emplace_back(normal);
	}

	for (const auto& tangent : otherData.tangents) {
		tangents.emplace_back(tangent);
	}

	for (const auto& texCoord : otherData.textureCoords) {
		textureCoords.emplace_back(texCoord);
	}
}

void MeshData::addData(const MeshData& otherData, const std::vector<glm::mat4>& offsets)
{
	assert(static_cast<unsigned long int>(positions.size()) + static_cast<unsigned long int>(otherData.positions.size()) * offsets.size() < static_cast<unsigned long int>(std::numeric_limits<unsigned int>::max()));

	indices.reserve(indices.size() + otherData.indices.size() * offsets.size());
	positions.reserve(positions.size() + otherData.positions.size() * offsets.size());
	normals.reserve(normals.size() + otherData.normals.size() * offsets.size());
	tangents.reserve(tangents.size() + otherData.tangents.size() * offsets.size());
	textureCoords.reserve(textureCoords.size() + otherData.textureCoords.size() * offsets.size());

	for (const auto& offset : offsets) {
		for (auto index : otherData.indices) {
			indices.emplace_back(static_cast<unsigned int>(positions.size()) + index);
		}

		for (const auto& position : otherData.positions) {
			positions.emplace_back(offset * glm::vec4(position, 1.0f));
		}

		for (const auto& normal : otherData.normals) {
			normals.emplace_back(normal);
		}

		for (const auto& tangent : otherData.tangents) {
			tangents.emplace_back(tangent);
		}

		for (const auto& texCoord : otherData.textureCoords) {
			textureCoords.emplace_back(texCoord);
		}
	}
}

MeshData createTriangle(float size)
{
    MeshData data;

    data.positions.reserve(3);
    data.positions.emplace_back(0, size / 2, 0);
    data.positions.emplace_back(size / 2, -size / 2, 0);
    data.positions.emplace_back(-size / 2, -size / 2, 0);

    data.normals.reserve(3);
    data.normals.emplace_back(0, 0, 1);
    data.normals.emplace_back(0, 0, 1);
    data.normals.emplace_back(0, 0, 1);

	data.tangents.reserve(3);
	data.tangents.emplace_back(1, 0, 0);
	data.tangents.emplace_back(1, 0, 0);
	data.tangents.emplace_back(1, 0, 0);

	data.textureCoords.reserve(3);
	data.textureCoords.emplace_back(0.5f, 1);
	data.textureCoords.emplace_back(1, 0);
	data.textureCoords.emplace_back(0, 0);

    data.indices.reserve(3);
    data.indices.push_back(0);
    data.indices.push_back(1);
    data.indices.push_back(2);

    return data;
}

MeshData createSphere(float radius, int stacks, int slices)
{
	const float PI05 = pi * 0.5f;
	const float PI2 = pi * 2.0f;

	MeshData data;

	// vertices
	for (int i = 0; i <= stacks; i++)
	{
		float v = static_cast<float>(i) / stacks;
		float phi = pi * v - PI05;
		for (int j = 0; j <= slices; j++)
		{
			float u = static_cast<float>(j) / slices;
			float theta = PI2 * u;

			auto& normal = data.normals.emplace_back(glm::vec3(cos(phi) * cos(theta), sin(phi), cos(phi) * sin(theta)));
			data.tangents.emplace_back(glm::vec3(sin(theta), 0.f, -cos(theta)));
			data.positions.push_back(normal * radius);
			data.textureCoords.push_back(glm::vec2(u, v));
		}
	}

	// indices
	for (int i = 0; i < stacks; i++)
	{
		for (int j = 0; j < slices; j++)
		{
			int offset = (slices + 1) * i + j;
			if (i != 0)
			{
				data.indices.push_back(offset);
				data.indices.push_back(offset + slices + 1);
				data.indices.push_back(offset + 1);
			}
			if (i != stacks - 1)
			{
				data.indices.push_back(offset + slices + 2);
				data.indices.push_back(offset + 1);
				data.indices.push_back(offset + slices + 1);
			}
		}
	}

	return data;
}
