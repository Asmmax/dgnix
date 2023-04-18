#pragma once
#include "resources/IShaderImpl.hpp"
#include "PoolAllocator.hpp"

class GLShaderImpl : public IShaderImpl
{
private:
	unsigned int _vertexShaderHandle;
	unsigned int _fragmentShaderHandle;
	unsigned int _programHandle;
	PoolAllocator<GLShaderImpl>* _allocator;

public:
	GLShaderImpl(PoolAllocator<GLShaderImpl>* allocator);

	void compileVertexShader(const std::string& shader) override;
	void compileFragmentShader(const std::string& shader) override;
	void link() override;

	void terminate() override;
	void free() override;

	void use() override;
	void clear() override;

	void setUniform(unsigned int location, const glm::mat4& matrix) override;
	void setUniform(unsigned int location, const glm::mat3& matrix) override;
	void setUniform(unsigned int location, const glm::vec4& vector) override;
	void setUniform(unsigned int location, const glm::vec3& vector) override;
	void setUniform(unsigned int location, float value) override;
	void setUniform(unsigned int location, int value) override;

	unsigned int getLocation(const StringId& name) const override;
};
