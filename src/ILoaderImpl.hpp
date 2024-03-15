#pragma once
#include <string>

class IMeshImpl;
class ITextureImpl;
class IShaderImpl;

class ILoaderImpl
{
public:
	ILoaderImpl() = default;
	ILoaderImpl(const ILoaderImpl& other) = delete;
	ILoaderImpl(ILoaderImpl&& other) = delete;
	ILoaderImpl& operator=(const ILoaderImpl& other) = delete;
	ILoaderImpl& operator=(ILoaderImpl&& other) = delete;

	virtual ~ILoaderImpl() = default;

	virtual IMeshImpl* createMesh() = 0;
	virtual ITextureImpl* createTexture() = 0;
	virtual IShaderImpl* createShader() = 0;
};
