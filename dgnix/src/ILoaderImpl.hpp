#pragma once
#include <string>

class IMeshImpl;
class ITextureImpl;
class IShaderImpl;

class ILoaderImpl
{
public:
	virtual ~ILoaderImpl() {}

	virtual IMeshImpl* createMesh() = 0;
	virtual ITextureImpl* createTexture() = 0;
	virtual IShaderImpl* createShader() = 0;
};
