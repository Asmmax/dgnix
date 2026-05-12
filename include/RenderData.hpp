#pragma once
#include "StringId.hpp"
#include "UnorderedMap.hpp"

#include <glm/fwd.hpp>

class Shader;

class RenderData
{
private:
	UnorderedMap<StringId, int> _intValues;
	UnorderedMap<StringId, float> _floatValues;
	UnorderedMap<StringId, glm::vec3> _vec3Values;
	UnorderedMap<StringId, glm::vec4> _vec4Values;
	UnorderedMap<StringId, glm::mat3> _mat3Values;
	UnorderedMap<StringId, glm::mat4> _mat4Values;

public:
	void setInt(StringId name, int value);
	void setFloat(StringId name, float value);
	void setVec3(StringId name, const glm::vec3& value);
	void setVec4(StringId name, const glm::vec4& value);
	void setMat3(StringId name, const glm::mat3& value);
	void setMat4(StringId name, const glm::mat4& value);

	int getInt(StringId name) const;
	float getFloat(StringId name) const;
	const glm::vec3& getVec3(StringId name) const;
	const glm::vec4& getVec4(StringId name) const;
	const glm::mat3& getMat3(StringId name) const;
	const glm::mat4& getMat4(StringId name) const;

	void apply(Shader& shader) const;
	void override(const RenderData& other);
	void clear();
};
