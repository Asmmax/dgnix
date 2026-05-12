#include "RenderData.hpp"

#include "resources/Shader.hpp"

void RenderData::setInt(StringId name, int value)
{
	if (auto* val = _intValues.getPtr(name)) {
		*val = value;
		return;
	}
	_intValues.add(name, value);
}

void RenderData::setFloat(StringId name, float value)
{
	if (auto* val = _floatValues.getPtr(name)) {
		*val = value;
		return;
	}
	_floatValues.add(name, value);
}

void RenderData::setVec3(StringId name, const glm::vec3& value)
{
	if (auto* val = _vec3Values.getPtr(name)) {
		*val = value;
		return;
	}
	_vec3Values.add(name, value);
}

void RenderData::setVec4(StringId name, const glm::vec4& value)
{
	if (auto* val = _vec4Values.getPtr(name)) {
		*val = value;
		return;
	}
	_vec4Values.add(name, value);
}

void RenderData::setMat3(StringId name, const glm::mat3& value)
{
	if (auto* val = _mat3Values.getPtr(name)) {
		*val = value;
		return;
	}
	_mat3Values.add(name, value);
}

void RenderData::setMat4(StringId name, const glm::mat4& value)
{
	if (auto* val = _mat4Values.getPtr(name)) {
		*val = value;
		return;
	}
	_mat4Values.add(name, value);
}

int RenderData::getInt(StringId name) const
{
	return _intValues.get(name);
}

float RenderData::getFloat(StringId name) const
{
	return _floatValues.get(name);
}

const glm::vec3& RenderData::getVec3(StringId name) const
{
	return _vec3Values.get(name);
}

const glm::vec4& RenderData::getVec4(StringId name) const
{
	return _vec4Values.get(name);
}

const glm::mat3& RenderData::getMat3(StringId name) const
{
	return _mat3Values.get(name);
}

const glm::mat4& RenderData::getMat4(StringId name) const
{
	return _mat4Values.get(name);
}

void RenderData::apply(Shader& shader) const
{
	for (size_t i = 0; i < _intValues.size(); i++) {
		auto location = shader.getLocation(_intValues.keys()[i]);
		if (location != static_cast<unsigned int>(-1)) {
			shader.setUniform(location, _intValues.values()[i]);
		}
	}
	for (size_t i = 0; i < _floatValues.size(); i++) {
		auto location = shader.getLocation(_floatValues.keys()[i]);
		if (location != static_cast<unsigned int>(-1)) {
			shader.setUniform(location, _floatValues.values()[i]);
		}
	}
	for (size_t i = 0; i < _vec3Values.size(); i++) {
		auto location = shader.getLocation(_vec3Values.keys()[i]);
		if (location != static_cast<unsigned int>(-1)) {
			shader.setUniform(location, _vec3Values.values()[i]);
		}
	}
	for (size_t i = 0; i < _vec4Values.size(); i++) {
		auto location = shader.getLocation(_vec4Values.keys()[i]);
		if (location != static_cast<unsigned int>(-1)) {
			shader.setUniform(location, _vec4Values.values()[i]);
		}
	}
	for (size_t i = 0; i < _mat3Values.size(); i++) {
		auto location = shader.getLocation(_mat3Values.keys()[i]);
		if (location != static_cast<unsigned int>(-1)) {
			shader.setUniform(location, _mat3Values.values()[i]);
		}
	}
	for (size_t i = 0; i < _mat4Values.size(); i++) {
		auto location = shader.getLocation(_mat4Values.keys()[i]);
		if (location != static_cast<unsigned int>(-1)) {
			shader.setUniform(location, _mat4Values.values()[i]);
		}
	}
}

void RenderData::override(const RenderData& other)
{
	for (size_t i = 0; i < other._intValues.size(); i++) {
		setInt(other._intValues.keys()[i], other._intValues.values()[i]);
	}
	for (size_t i = 0; i < other._floatValues.size(); i++) {
		setFloat(other._floatValues.keys()[i], other._floatValues.values()[i]);
	}
	for (size_t i = 0; i < other._vec3Values.size(); i++) {
		setVec3(other._vec3Values.keys()[i], other._vec3Values.values()[i]);
	}
	for (size_t i = 0; i < other._vec4Values.size(); i++) {
		setVec4(other._vec4Values.keys()[i], other._vec4Values.values()[i]);
	}
	for (size_t i = 0; i < other._mat3Values.size(); i++) {
		setMat3(other._mat3Values.keys()[i], other._mat3Values.values()[i]);
	}
	for (size_t i = 0; i < other._mat4Values.size(); i++) {
		setMat4(other._mat4Values.keys()[i], other._mat4Values.values()[i]);
	}
}

void RenderData::clear()
{
	_intValues.clear();
	_floatValues.clear();
	_vec3Values.clear();
	_vec4Values.clear();
	_mat3Values.clear();
	_mat4Values.clear();
}
