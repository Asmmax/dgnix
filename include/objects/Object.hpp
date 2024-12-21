#pragma once
#include "DrawState.hpp"
#include <glm/mat4x4.hpp>

class Mesh;
class Shader;

class Object
{
private:
	glm::mat4 _matrix;
	Mesh* _mesh;
	DrawStateDef _state;

public:
	Object();

	void setMesh(Mesh* mesh);

	void setMatrix(const glm::mat4& matrix);
	const glm::mat4& getMatrix() const { return _matrix; }

	DrawStateDef& getState() { return _state; }
	const DrawStateDef& getState() const { return _state; }

	bool isCaughtIntoView(const glm::mat4& viewProjMatrix) const;
	void draw(Shader* shader, const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
};
