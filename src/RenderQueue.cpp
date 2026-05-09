#include "RenderQueue.hpp"

#include "StringId.hpp"
#include "Material.hpp"
#include "resources/Shader.hpp"
#include "resources/Mesh.hpp"

static const StringId VIEW_MATRIX_NAME("ViewMatrix");
static const StringId PROJ_MATRIX_NAME("ProjectionMatrix");
static const StringId MODEL_MATRIX_NAME("ModelMatrix");
static const StringId MV_MATRIX_NAME("ModelViewMatrix");
static const StringId MVP_MATRIX_NAME("MVP");
static const StringId NORMAL_MATRIX_NAME("NormalMatrix");

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

static void fillModelMatrixData(RenderData& modelMatrixData, const RenderData& frameData, const glm::mat4& matrix)
{
	modelMatrixData.setMat4(MODEL_MATRIX_NAME, matrix);

	const auto& viewMatrix = frameData.getMat4(VIEW_MATRIX_NAME);
	const auto& projMatrix = frameData.getMat4(PROJ_MATRIX_NAME);

	const auto mvMatrix = viewMatrix * matrix;
	modelMatrixData.setMat4(MV_MATRIX_NAME, mvMatrix);

	const auto mvpMatrix = projMatrix * mvMatrix;
	modelMatrixData.setMat4(MVP_MATRIX_NAME, mvpMatrix);

	const glm::mat3 modelMatrix3x3 = matrix;
	const auto normalMatrix = glm::transpose(glm::inverse(modelMatrix3x3));
	modelMatrixData.setMat3(NORMAL_MATRIX_NAME, normalMatrix);
}

void RenderQueue::execute(const RenderData& frameData) const
{
	if (_commands.empty()) {
		return;
	}

	Shader* currentShader = nullptr;
	const Material* currentMaterial = nullptr;
	RenderData matrixModelData;

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

		matrixModelData.clear();
		fillModelMatrixData(matrixModelData, frameData, command.modelMatrix);
		matrixModelData.apply(*currentShader);

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
