#include "GLMeshImpl.hpp"
#include "resources/MeshData.hpp"
#include "gl/gl_core_4_3.hpp"
#include <glm/vec3.hpp>

GLMeshImpl::GLMeshImpl(PoolAllocator<GLMeshImpl>* allocator):
	_mode(0),
	_vaoHandle(0),
	_allocator(allocator)
{
}

void GLMeshImpl::init()
{
	_vboHandles.resize(5);
	gl::GenBuffers(5, _vboHandles.data());

	const GLuint posBufHandle = _vboHandles[0];
	const GLuint normBufHandle = _vboHandles[1];
	const GLuint tangBufHandle = _vboHandles[2];
	const GLuint uvBufHandle = _vboHandles[3];
	const GLuint elemBufHandle = _vboHandles[4];

	gl::GenVertexArrays(1, &_vaoHandle);
	gl::BindVertexArray(_vaoHandle);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, elemBufHandle);

	gl::EnableVertexAttribArray(0);
	gl::EnableVertexAttribArray(1);
	gl::EnableVertexAttribArray(2);
	gl::EnableVertexAttribArray(3);

	gl::BindBuffer(gl::ARRAY_BUFFER, posBufHandle);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, nullptr);

	gl::BindBuffer(gl::ARRAY_BUFFER, normBufHandle);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE_, 0, nullptr);

	gl::BindBuffer(gl::ARRAY_BUFFER, tangBufHandle);
	gl::VertexAttribPointer(2, 3, gl::FLOAT, gl::FALSE_, 0, nullptr);

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufHandle);
	gl::VertexAttribPointer(3, 2, gl::FLOAT, gl::FALSE_, 0, nullptr);

	gl::BindVertexArray(0);
}

void GLMeshImpl::terminate()
{
	gl::DeleteVertexArrays(1, &_vaoHandle);
	_vaoHandle = 0;

	gl::DeleteBuffers(static_cast<GLsizei>(_vboHandles.size()), _vboHandles.data());
	_vboHandles.clear();
}

void GLMeshImpl::free()
{
	PoolAllocator<GLMeshImpl>* tempAllocator = _allocator;
	tempAllocator->destroy(this);
	tempAllocator->deallocate(this);
}

void GLMeshImpl::draw(size_t indicesCount)
{
	gl::BindVertexArray(_vaoHandle);

	gl::DrawElements(static_cast<GLenum>(_mode), static_cast<int>(indicesCount), gl::UNSIGNED_INT, nullptr);

	gl::BindVertexArray(0);
}

void GLMeshImpl::updateData(const MeshData& data)
{
	const GLuint posBufHandle = _vboHandles[0];
	const GLuint normBufHandle = _vboHandles[1];
	const GLuint tangBufHandle = _vboHandles[2];
	const GLuint uvBufHandle = _vboHandles[3];
	const GLuint elemBufHandle = _vboHandles[4];

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, elemBufHandle);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, posBufHandle);
	gl::BufferData(gl::ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, normBufHandle);
	gl::BufferData(gl::ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), data.normals.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, tangBufHandle);
	gl::BufferData(gl::ARRAY_BUFFER, data.tangents.size() * sizeof(glm::vec3), data.tangents.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, uvBufHandle);
	gl::BufferData(gl::ARRAY_BUFFER, data.textureCoords.size() * sizeof(glm::vec2), data.textureCoords.data(), gl::STATIC_DRAW);

	switch (data.primitiveSize)
	{
	case 1:
		_mode = gl::POINTS;
		break;
	case 2:
		_mode = gl::LINES;
		break;
	case 3:
		_mode = gl::TRIANGLES;
		break;
	case 4:
		_mode = gl::QUADS;
		break;
	default:
		_mode = 0;
		break;
	}
}
