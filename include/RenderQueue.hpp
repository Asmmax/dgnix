#pragma once
#include "RenderData.hpp"

#include <glm/mat4x4.hpp>
#include <vector>

class Mesh;
class Shader;
class Material;

struct RenderCommand
{
	Shader* shader{};
	const Material* material{};
	Mesh* mesh{};
	RenderData objectData{};
	glm::mat4 modelMatrix{};
};

class RenderQueue
{
private:
	std::vector<RenderCommand> _commands;

public:
	void submit(const RenderCommand& command);
	void sort();
	void execute(const RenderData& frameData) const;
	void clear();
};
