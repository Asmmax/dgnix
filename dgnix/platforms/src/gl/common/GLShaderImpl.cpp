#include "GLShaderImpl.hpp"
#include "gl/gl_core_4_3.hpp"
#include <glm/gtc/type_ptr.hpp>

GLShaderImpl::GLShaderImpl(PoolAllocator<GLShaderImpl>* allocator):
	_vertexShaderHandle(0),
	_fragmentShaderHandle(0),
	_programHandle(0),
	_allocator(allocator)
{
}

void GLShaderImpl::compileVertexShader(const std::string& shader)
{
	if (shader.empty()) {
		fprintf(stderr, "Vertex shader is None.\n");
		return;
	}

	_vertexShaderHandle = gl::CreateShader(gl::VERTEX_SHADER);
	if (_vertexShaderHandle == 0) {
		fprintf(stderr, "Error creating vertex shader.\n");
		return;
	}

	const GLchar* shaderCode = shader.c_str();
	const GLchar* codeArray[] = { shaderCode };
	gl::ShaderSource(_vertexShaderHandle, 1, codeArray, NULL);

	gl::CompileShader(_vertexShaderHandle);

	GLint result = gl::FALSE_;
	gl::GetShaderiv(_vertexShaderHandle, gl::COMPILE_STATUS, &result);
	if (result == gl::FALSE_) {
		fprintf(stderr, "Vertex shader compilation failed!\n");

		GLint logLen = 0;
		gl::GetShaderiv(_vertexShaderHandle, gl::INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			GLchar* log = new char[logLen];
			GLsizei written = 0;
			gl::GetShaderInfoLog(_vertexShaderHandle, logLen, &written, log);

			fprintf(stderr, "Shader log:\n%s", log);
			delete[] log;
		}
	}
}

void GLShaderImpl::compileFragmentShader(const std::string& shader)
{
	if (shader.empty()) {
		fprintf(stderr, "Fragment shader is None.\n");
		return;
	}

	_fragmentShaderHandle = gl::CreateShader(gl::FRAGMENT_SHADER);
	if (_fragmentShaderHandle == 0) {
		fprintf(stderr, "Error creating fragment shader.\n");
		return;
	}

	const GLchar* shaderCode = shader.c_str();
	const GLchar* codeArray[] = { shaderCode };
	gl::ShaderSource(_fragmentShaderHandle, 1, codeArray, NULL);

	gl::CompileShader(_fragmentShaderHandle);

	GLint result = gl::FALSE_;
	gl::GetShaderiv(_fragmentShaderHandle, gl::COMPILE_STATUS, &result);
	if (result == gl::FALSE_) {
		fprintf(stderr, "Fragment shader compilation failed!\n");

		GLint logLen = 0;
		gl::GetShaderiv(_fragmentShaderHandle, gl::INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			GLchar* log = new char[logLen];
			GLsizei written = 0;
			gl::GetShaderInfoLog(_fragmentShaderHandle, logLen, &written, log);

			fprintf(stderr, "Shader log:\n%s", log);
			delete[] log;
		}
	}
}

void GLShaderImpl::link()
{
	_programHandle = gl::CreateProgram();
	if (_programHandle == 0) {
		fprintf(stderr, "Error creating shader program.\n");
		return;
	}

	if (_vertexShaderHandle != 0) {
		gl::AttachShader(_programHandle, _vertexShaderHandle);
	}

	if (_fragmentShaderHandle != 0) {
		gl::AttachShader(_programHandle, _fragmentShaderHandle);
	}

	gl::LinkProgram(_programHandle);

	GLint result = gl::FALSE_;
	gl::GetProgramiv(_programHandle, gl::LINK_STATUS, &result);
	if (result == gl::FALSE_) {
		fprintf(stderr, "Failed to link shader program!\n");

		GLint logLen = 0;
		gl::GetProgramiv(_programHandle, gl::INFO_LOG_LENGTH, &logLen);
		if (logLen > 0) {
			GLchar* log = new char[logLen];
			GLsizei written = 0;
			gl::GetProgramInfoLog(_programHandle, logLen, &written, log);

			fprintf(stderr, "Program log:\n%s", log);
			delete[] log;
		}
	}

	if (_vertexShaderHandle != 0) {
		gl::DetachShader(_programHandle, _vertexShaderHandle);
		gl::DeleteShader(_vertexShaderHandle);
		_vertexShaderHandle = 0;
	}

	if (_fragmentShaderHandle != 0) {
		gl::DetachShader(_programHandle, _fragmentShaderHandle);
		gl::DeleteShader(_fragmentShaderHandle);
		_fragmentShaderHandle = 0;
	}
}

void GLShaderImpl::terminate()
{
	gl::DeleteProgram(_programHandle);
	_programHandle = 0;
}

void GLShaderImpl::free()
{
	PoolAllocator<GLShaderImpl>* tempAllocator = _allocator;
	tempAllocator->destroy(this);
	tempAllocator->deallocate(this);
}

void GLShaderImpl::use()
{
	gl::UseProgram(_programHandle);
}

void GLShaderImpl::clear()
{
	gl::UseProgram(0);
}

void GLShaderImpl::setUniform(unsigned int location, const glm::mat4& matrix)
{
	gl::UniformMatrix4fv(location, 1, gl::FALSE_, glm::value_ptr(matrix));
}

void GLShaderImpl::setUniform(unsigned int location, const glm::mat3& matrix)
{
	gl::UniformMatrix3fv(location, 1, gl::FALSE_, glm::value_ptr(matrix));
}

void GLShaderImpl::setUniform(unsigned int location, const glm::vec4& vector)
{
	gl::Uniform4fv(location, 1, glm::value_ptr(vector));
}

void GLShaderImpl::setUniform(unsigned int location, const glm::vec3& vector)
{
	gl::Uniform3fv(location, 1, glm::value_ptr(vector));
}

void GLShaderImpl::setUniform(unsigned int location, float value)
{
	gl::Uniform1f(location, value);
}

void GLShaderImpl::setUniform(unsigned int location, int value)
{
	gl::Uniform1i(location, value);
}

unsigned int GLShaderImpl::getLocation(const StringId& name) const
{
	return gl::GetUniformLocation(_programHandle, name.getChars());
}
