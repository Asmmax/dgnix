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

	auto mvpMatrix = viewProjMatrix * _matrix;

	auto& bbox = _mesh->getBoundingBox();
	BBox projectedBbox = bbox.project(mvpMatrix);

	static BBox screenBbox = { -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 2.0f };

	return projectedBbox.isOverlapped(screenBbox, 1e-4f);
}

void Object::draw(Shader* shader, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	if (!_mesh) {
		return;
	}

	auto mvMatrix = viewMatrix * _matrix;
	auto mvpMatrix = projMatrix * mvMatrix;

	auto normalMatrix4x4 = glm::transpose(glm::inverse(mvMatrix));
	glm::mat3 normalMatrix(normalMatrix4x4);

	static StringId modelMatrixName = StringId("ModelMatrix");
	auto modelMatrixLocation = shader->getLocation(modelMatrixName);
	if (modelMatrixLocation != -1) {
		shader->setUniform(modelMatrixLocation, _matrix);
	}

	static StringId mvMatrixName = StringId("ModelViewMatrix");
	auto mvMatrixLocation = shader->getLocation(mvMatrixName);
	if (mvMatrixLocation != -1) {
		shader->setUniform(mvMatrixLocation, mvMatrix);
	}

	static StringId mvpMatrixName = StringId("MVP");
	auto mvpMatrixLocation = shader->getLocation(mvpMatrixName);
	if (mvpMatrixLocation != -1) {
		shader->setUniform(mvpMatrixLocation, mvpMatrix);
	}

	static StringId normalMatrixName = StringId("NormalMatrix");
	auto normalMatrixLocation = shader->getLocation(normalMatrixName);
	if (normalMatrixLocation != -1) {
		shader->setUniform(normalMatrixLocation, normalMatrix);
	}

	_mesh->draw();
}
