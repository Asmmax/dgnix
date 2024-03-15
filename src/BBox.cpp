#include "BBox.hpp"
#include "CommonMath.hpp"
#include <array>

void BBox::addPoint(float x, float y, float z)
{
	xMin = MIN(x, xMin);
	yMin = MIN(y, yMin);
	zMin = MIN(z, zMin);

	xMax = MAX(x, xMax);
	yMax = MAX(y, yMax);
	zMax = MAX(z, zMax);
}

void BBox::addPoint(const glm::vec3& point)
{
	xMin = MIN(point.x, xMin);
	yMin = MIN(point.y, yMin);
	zMin = MIN(point.z, zMin);

	xMax = MAX(point.x, xMax);
	yMax = MAX(point.y, yMax);
	zMax = MAX(point.z, zMax);
}

BBox BBox::project(const glm::mat4& projMatrix) const
{
	const std::array<glm::vec4, 8> vertices = {
		glm::vec4{xMin, yMin, zMin, 1.0f},
		glm::vec4{xMin, yMin, zMax, 1.0f},
		glm::vec4{xMin, yMax, zMin, 1.0f},
		glm::vec4{xMin, yMax, zMax, 1.0f},
		glm::vec4{xMax, yMin, zMin, 1.0f},
		glm::vec4{xMax, yMin, zMax, 1.0f},
		glm::vec4{xMax, yMax, zMin, 1.0f},
		glm::vec4{xMax, yMax, zMax, 1.0f}
	};

	std::array<glm::vec4, 8> projectedVertices;
	for (int i = 0; i < 8; i++) {
		projectedVertices[i] = projMatrix * vertices[i];
	}

	BBox projectedBounds;
	for (auto& projectedVertex : projectedVertices) {
		if (projectedVertex.w < 0.0f) {
			projectedBounds.addPoint(0.0f, 0.0f, 0.0f);
			continue;
		}
		projectedBounds.addPoint(projectedVertex.x / projectedVertex.w, projectedVertex.y / projectedVertex.w, projectedVertex.z / projectedVertex.w);
	}

	return projectedBounds;
}

bool BBox::isOverlapped(const BBox& other, float epsilon) const
{
	return !(FLOAT_IS_GREATER(xMin, other.xMax, epsilon)
		|| FLOAT_IS_LESS(xMax, other.xMin, epsilon)
		|| FLOAT_IS_GREATER(yMin, other.yMax, epsilon)
		|| FLOAT_IS_LESS(yMax, other.yMin, epsilon)
		|| FLOAT_IS_GREATER(zMin, other.zMax, epsilon)
		|| FLOAT_IS_LESS(zMax, other.zMin, epsilon));
}
