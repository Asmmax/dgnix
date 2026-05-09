#include "objects/Object.hpp"

#include "RenderQueue.hpp"
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

void Object::render(RenderQueue& renderQueue, Shader* shader, const Material* material)
{
	if (!_mesh) {
		return;
	}

	RenderCommand cmd{};

	cmd.shader = shader;
	cmd.material = material;
	cmd.mesh = _mesh;
	cmd.modelMatrix = _matrix;

	_state.fill(cmd.objectData);

	renderQueue.submit(cmd);
}
