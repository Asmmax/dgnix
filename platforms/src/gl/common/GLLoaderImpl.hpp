#pragma once
#include "ILoaderImpl.hpp"
#include "GLMeshImpl.hpp"
#include "GLTextureImpl.hpp"
#include "GLShaderImpl.hpp"
#include "PoolAllocator.hpp"

class GLLoaderImpl : public ILoaderImpl
{
	PoolAllocator<GLMeshImpl> _meshAllocator;
	PoolAllocator<GLTextureImpl> _textureAllocator;
	PoolAllocator<GLShaderImpl> _shaderAllocator;

public:
	GLLoaderImpl(size_t poolSize = 100);

	IMeshImpl* createMesh() override;
	ITextureImpl* createTexture() override;
	IShaderImpl* createShader() override;
};
