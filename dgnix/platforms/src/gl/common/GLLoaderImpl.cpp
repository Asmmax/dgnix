#include "GLLoaderImpl.hpp"
#include <cstdint>

GLLoaderImpl::GLLoaderImpl(size_t poolSize /*= 100*/):
	_meshAllocator(poolSize),
	_textureAllocator(poolSize),
	_shaderAllocator(poolSize)
{
}

IMeshImpl* GLLoaderImpl::createMesh()
{
	GLMeshImpl* newMesh = _meshAllocator.allocate();
	_meshAllocator.construct(newMesh, &_meshAllocator);
	return newMesh;
}

ITextureImpl* GLLoaderImpl::createTexture()
{
	GLTextureImpl* newTexture = _textureAllocator.allocate();
	_textureAllocator.construct(newTexture, &_textureAllocator);
	return newTexture;
}

IShaderImpl* GLLoaderImpl::createShader()
{
	GLShaderImpl* newShader = _shaderAllocator.allocate();
	_shaderAllocator.construct(newShader, &_shaderAllocator);
	return newShader;
}
