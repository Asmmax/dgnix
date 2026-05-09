#include "RenderQueue.hpp"

#include "StringId.hpp"
#include "Material.hpp"
#include "resources/Shader.hpp"
#include "resources/Mesh.hpp"

void RenderQueue::submit(const RenderCommand& command)
{
	_commands.push_back(command);
}

void RenderQueue::sort()
{
	std::sort(_commands.begin(), _commands.end(), [](const RenderCommand& a, const RenderCommand& b) {
		if (a.shader != b.shader) {
			return a.shader < b.shader;
		}
		if (a.material != b.material) {
			return a.material < b.material;
		}
		return a.mesh < b.mesh;
	});
}

static RenderData computeModelMatrixData(const RenderData& frameData, const glm::mat4& matrix)
{
	static const StringId viewMatrixName = StringId("ViewMatrix");
	const auto& viewMatrix = frameData.getMat4(viewMatrixName);

	static const StringId projMatrixName = StringId("ProjectionMatrix");
	const auto& projMatrix = frameData.getMat4(projMatrixName);

	const auto mvMatrix = viewMatrix * matrix;
	const auto mvpMatrix = projMatrix * mvMatrix;

	const glm::mat3 modelMatrix3x3 = matrix;
	const auto normalMatrix = glm::transpose(glm::inverse(modelMatrix3x3));

	const glm::mat3 viewMatrix3x3 = viewMatrix;
	const glm::vec3 viewOrigin = -glm::inverse(viewMatrix3x3) * viewMatrix[3];

	RenderData result;

	static const StringId modelMatrixName = StringId("ModelMatrix");
	result.setMat4(modelMatrixName, matrix);

	static const StringId mvMatrixName = StringId("ModelViewMatrix");
	result.setMat4(mvMatrixName, mvMatrix);

	static const StringId mvpMatrixName = StringId("MVP");
	result.setMat4(mvpMatrixName, mvpMatrix);

	static const StringId normalMatrixName = StringId("NormalMatrix");
	result.setMat3(normalMatrixName, normalMatrix);

	static const StringId viewOriginName = StringId("ViewOrigin");
	result.setVec3(viewOriginName, viewOrigin);

	return result;
}

void RenderQueue::execute(const RenderData& frameData) const
{
	if (_commands.empty()) {
		return;
	}

	Shader* currentShader = nullptr;
	const Material* currentMaterial = nullptr;

	for (const RenderCommand& command : _commands) {
		bool shaderChanged = command.shader != currentShader;
		if (shaderChanged) {
			if (currentShader) {
				currentShader->clear();
			}
			currentShader = command.shader;
		}

		if (!currentShader) {
			continue;
		}

		if (shaderChanged) {
			currentShader->use();
			frameData.apply(*currentShader);
			currentMaterial = nullptr;
		}

		if (!command.material) {
			currentMaterial = nullptr;
			continue;
		}

		bool materialChanged = command.material != currentMaterial;
		if (materialChanged) {
			currentMaterial = command.material;
			currentMaterial->apply(*currentShader);
		}

		command.objectData.apply(*currentShader);

		computeModelMatrixData(frameData, command.modelMatrix).apply(*currentShader);

		command.mesh->draw();
	}

	if (currentShader) {
		currentShader->clear();
	}
}

void RenderQueue::clear()
{
	_commands.clear();
}
