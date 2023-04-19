#pragma once
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

struct BBox
{
	float xMin = std::numeric_limits<float>::max();
	float yMin = std::numeric_limits<float>::max();
	float zMin = std::numeric_limits<float>::max();
	float xMax = std::numeric_limits<float>::lowest();
	float yMax = std::numeric_limits<float>::lowest();
	float zMax = std::numeric_limits<float>::lowest();

	void addPoint(float x, float y, float z);
	void addPoint(const glm::vec3& point);
	BBox project(const glm::mat4& projMatrix) const;
	bool isOverlapped(const BBox& other, float epsilon = std::numeric_limits<float>::epsilon()) const;
};
