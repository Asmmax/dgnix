#include "GLMeshImpl.hpp"
#include "resources/MeshData.hpp"
#include "gl/gl_core_4_3.hpp"
#include <glm/vec3.hpp>

GLMeshImpl::GLMeshImpl(PoolAllocator<GLMeshImpl>* allocator):
	_vaoHandle(0),
	_allocator(allocator)
{
}

void GLMeshImpl::init()
{
	_vboHandles.resize(3);
	gl::GenBuffers(3, &_vboHandles[0]);

	const GLuint posBufHandle = _vboHandles[0];
	const GLuint normBufHandle = _vboHandles[1];
	const GLuint elemBufHandle = _vboHandles[2];

	gl::GenVertexArrays(1, &_vaoHandle);
	gl::BindVertexArray(_vaoHandle);

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, elemBufHandle);

	gl::EnableVertexAttribArray(0);
	gl::EnableVertexAttribArray(1);

	gl::BindBuffer(gl::ARRAY_BUFFER, posBufHandle);
	gl::VertexAttribPointer(0, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

	gl::BindBuffer(gl::ARRAY_BUFFER, normBufHandle);
	gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE_, 0, NULL);

	gl::BindVertexArray(0);
}

void GLMeshImpl::terminate()
{
	gl::DeleteVertexArrays(1, &_vaoHandle);
	_vaoHandle = 0;

	gl::DeleteBuffers(static_cast<GLsizei>(_vboHandles.size()), &_vboHandles[0]);
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

	gl::DrawElements(gl::TRIANGLES, static_cast<int>(indicesCount), gl::UNSIGNED_INT, 0);

	gl::BindVertexArray(0);
}

void GLMeshImpl::updateData(const MeshData& data)
{
	const GLuint posBufHandle = _vboHandles[0];
	const GLuint normBufHandle = _vboHandles[1];
	const GLuint elemBufHandle = _vboHandles[2];

	gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, elemBufHandle);
	gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, data.indices.size() * sizeof(unsigned int), data.indices.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, posBufHandle);
	gl::BufferData(gl::ARRAY_BUFFER, data.positions.size() * sizeof(glm::vec3), data.positions.data(), gl::STATIC_DRAW);

	gl::BindBuffer(gl::ARRAY_BUFFER, normBufHandle);
	gl::BufferData(gl::ARRAY_BUFFER, data.normals.size() * sizeof(glm::vec3), data.normals.data(), gl::STATIC_DRAW);
}
