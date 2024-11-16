#include "objects/Object.hpp"
#include "resources/Mesh.hpp"
#include "StringId.hpp"
#include "resources/Shader.hpp"

Object::Object():
	_matrix(),
	_mesh(nullptr)
{
}

void Object::setMesh(Mesh* mesh)
{
	_mesh = mesh;
}

void Object::setMatrix(const glm::mat4& matrix)
{
	_matrix = matrix;
}

bool Object::isCaughtIntoView(const glm::mat4& viewProjMatrix) const
{
	if (!_mesh) {
		return false;
	}

	const auto mvpMatrix = viewProjMatrix * _matrix;

	const auto& bbox = _mesh->getBoundingBox();
	const BBox projectedBbox = bbox.project(mvpMatrix);

	static const BBox screenBbox = { -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 2.0f };

	return projectedBbox.isOverlapped(screenBbox, 1e-4f);
}

void Object::draw(Shader* shader, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	if (!_mesh) {
		return;
	}

	const auto mvMatrix = viewMatrix * _matrix;
	const auto mvpMatrix = projMatrix * mvMatrix;

	const glm::mat3 modelMatrix3x3 = _matrix;
	const auto normalMatrix = glm::transpose(glm::inverse(modelMatrix3x3));

	const glm::mat3 viewMatrix3x3 = viewMatrix;
	const glm::vec3 viewOrigin = -glm::inverse(viewMatrix3x3) * viewMatrix[3];

	static const StringId modelMatrixName = StringId("ModelMatrix");
	const auto modelMatrixLocation = shader->getLocation(modelMatrixName);
	if (modelMatrixLocation != static_cast<unsigned int>(-1)) {
		shader->setUniform(modelMatrixLocation, _matrix);
	}

	static const StringId mvMatrixName = StringId("ModelViewMatrix");
	const auto mvMatrixLocation = shader->getLocation(mvMatrixName);
	if (mvMatrixLocation != static_cast<unsigned int>(-1)) {
		shader->setUniform(mvMatrixLocation, mvMatrix);
	}

	static const StringId mvpMatrixName = StringId("MVP");
	const auto mvpMatrixLocation = shader->getLocation(mvpMatrixName);
	if (mvpMatrixLocation != static_cast<unsigned int>(-1)) {
		shader->setUniform(mvpMatrixLocation, mvpMatrix);
	}

	static const StringId normalMatrixName = StringId("NormalMatrix");
	const auto normalMatrixLocation = shader->getLocation(normalMatrixName);
	if (normalMatrixLocation != static_cast<unsigned int>(-1)) {
		shader->setUniform(normalMatrixLocation, normalMatrix);
	}

	static const StringId viewOriginName = StringId("ViewOrigin");
	const auto viewOriginLocation = shader->getLocation(viewOriginName);
	if (viewOriginLocation != static_cast<unsigned int>(-1)) {
		shader->setUniform(viewOriginLocation, viewOrigin);
	}

	_mesh->draw();
}
