#include "resources/Shader.hpp"
#include "resources/IShaderImpl.hpp"

Shader::Shader(IShaderImpl* shaderImpl):
	_impl(shaderImpl)
{
}

Shader::~Shader()
{
	_impl->terminate();
	_impl->free();
}

void Shader::init(const std::string& vertexShader, const std::string& fragmentShader)
{
	_impl->compileVertexShader(vertexShader);
	_impl->compileFragmentShader(fragmentShader);
	_impl->link();
}

void Shader::use()
{
	_impl->use();
}

void Shader::clear()
{
	_impl->clear();
}

void Shader::setUniform(unsigned int location, const glm::mat4& matrix)
{
	_impl->setUniform(location, matrix);
}

void Shader::setUniform(unsigned int location, const glm::mat3& matrix)
{
	_impl->setUniform(location, matrix);
}

void Shader::setUniform(unsigned int location, const glm::vec4& vector)
{
	_impl->setUniform(location, vector);
}

void Shader::setUniform(unsigned int location, const glm::vec3& vector)
{
	_impl->setUniform(location, vector);
}

void Shader::setUniform(unsigned int location, float value)
{
	_impl->setUniform(location, value);
}

void Shader::setUniform(unsigned int location, int value)
{
	_impl->setUniform(location, value);
}

unsigned int Shader::getLocation(const StringId& name) const
{
	return _impl->getLocation(name);
}
