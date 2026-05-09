#pragma once
#include "RenderData.hpp"
#include "StringId.hpp"
#include "UnorderedMap.hpp"

class Texture;
class Shader;

class Material
{
private:
	RenderData _data;
	UnorderedMap<StringId, Texture*> _textures;

public:
	void setTexture(StringId name, Texture* texture);
	void removeTexture(StringId name);

	void apply(Shader& shader) const;

	RenderData& getMaterialData();
	const RenderData& getMaterialData() const;
};
