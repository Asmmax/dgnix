#include "resources/MeshData.hpp"
#include "glm/gtx/normal.hpp"


void MeshData::addData(const MeshData& otherData, const glm::mat4& offset)
{
	assert(static_cast<unsigned long int>(positions.size()) + static_cast<unsigned long int>(otherData.positions.size()) < static_cast<unsigned long int>(std::numeric_limits<unsigned int>::max()));

	indices.reserve(indices.size() + otherData.indices.size());
	positions.reserve(positions.size() + otherData.positions.size());
	normals.reserve(normals.size() + otherData.normals.size());

	for (auto index : otherData.indices) {
		indices.emplace_back(static_cast<unsigned int>(positions.size()) + index);
	}
	
	for (auto& position : otherData.positions) {
		positions.emplace_back(offset * glm::vec4(position, 1.0f));
	}
	
	for (auto& normal : otherData.normals) {
		normals.emplace_back(normal);
	}
}

void MeshData::addData(const MeshData& otherData, const std::vector<glm::mat4>& offsets)
{
	assert(static_cast<unsigned long int>(positions.size()) + static_cast<unsigned long int>(otherData.positions.size()) * offsets.size() < static_cast<unsigned long int>(std::numeric_limits<unsigned int>::max()));

	indices.reserve(indices.size() + otherData.indices.size() * offsets.size());
	positions.reserve(positions.size() + otherData.positions.size() * offsets.size());
	normals.reserve(normals.size() + otherData.normals.size() * offsets.size());

	for (auto& offset : offsets) {
		for (auto index : otherData.indices) {
			indices.emplace_back(static_cast<unsigned int>(positions.size()) + index);
		}

		positions.reserve(positions.size() + otherData.positions.size());
		for (auto& position : otherData.positions) {
			positions.emplace_back(offset * glm::vec4(position, 1.0f));
		}

		normals.reserve(normals.size() + otherData.normals.size());
		for (auto& normal : otherData.normals) {
			normals.emplace_back(normal);
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

    data.indices.reserve(3);
    data.indices.push_back(0);
    data.indices.push_back(1);
    data.indices.push_back(2);

    return data;
}

MeshData createSphere(float radius, unsigned int segCount, float fStart, float fEnd)
{
	MeshData data;

	int V = segCount + 1;
	int G = segCount * 2 + 1;
	data.positions.resize(V * G);
	float theta, phi;
	int cnt = 0;
	for (int i = 0; i < V; i++) {
		phi = fStart + (fEnd - fStart) * i / (V - 1);
		for (int j = 0; j < G; j++) {
			theta = 2 * pi * j / (G - 1);
			data.positions[cnt] = radius * glm::vec3(cos(phi) * cos(theta),
				sin(phi),
				cos(phi) * sin(theta));
			cnt++;
		};
	};
	data.indices.resize(6 * (V - 1) * (G - 1));
#define SET(x,y) ((x)*(G)+(y))
	for (int i = 0; i < V - 1; i++) {
		for (int j = 0; j < G - 1; j++) {
			data.indices[i * (G - 1) * 6 + j * 6 + 0] = SET(i + 0, j + 0);
			data.indices[i * (G - 1) * 6 + j * 6 + 2] = SET(i + 1, j + 1);
			data.indices[i * (G - 1) * 6 + j * 6 + 1] = SET(i + 1, j + 0);
			data.indices[i * (G - 1) * 6 + j * 6 + 3] = SET(i + 0, j + 0);
			data.indices[i * (G - 1) * 6 + j * 6 + 5] = SET(i + 0, j + 1);
			data.indices[i * (G - 1) * 6 + j * 6 + 4] = SET(i + 1, j + 1);
		};
	};
#undef SET


	data.normals.clear();
	data.normals.reserve(data.positions.size());

	for (auto& vertex : data.positions) {
		data.normals.push_back(glm::normalize(vertex));
	}

	return data;
}
