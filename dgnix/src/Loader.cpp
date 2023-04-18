#include "Loader.hpp"
#include "ILoaderImpl.hpp"
#include "resources/IMeshImpl.hpp"
#include "resources/ITextureImpl.hpp"
#include "resources/IShaderImpl.hpp"
#include "resources/TextureData.hpp"
#include "IGraphicsContext.hpp"
#include <assert.h>

Loader::Loader(ILoaderImpl* loaderImpl, size_t poolSize /*= 100*/):
	_impl(loaderImpl),
	_context(nullptr),
	_meshAllocator(poolSize),
	_textureAllocator(poolSize),
	_shaderAllocator(poolSize)
{
	//Allocate memory for vectors in advance
	_meshes.reserve(poolSize);
	_textures.reserve(poolSize);
	_shaders.reserve(poolSize);
}

Loader::~Loader()
{
	if (!_context) {
		return;
	}
	_context->makeCurrent();

	for (auto meshPtr : _meshes) {
		_meshAllocator.destroy(meshPtr);
	}

	for (auto texturePtr : _textures) {
		_textureAllocator.destroy(texturePtr);
	}

	for (auto shaderPtr : _shaders) {
		_shaderAllocator.destroy(shaderPtr);
	}

	delete _impl;
}

void Loader::init(IGraphicsContext* context)
{
	_context = context;
}

Mesh* Loader::loadMesh(const MeshData& data)
{
	if (!_context) {
		return nullptr;
	}
	_context->makeCurrent();

	auto meshImpl = _impl->createMesh();
	if (!meshImpl) {
		return nullptr;
	}

	Mesh* newMesh = _meshAllocator.allocate();
	_meshAllocator.construct(newMesh, meshImpl);
	newMesh->init(data);
	_meshes.push_back(newMesh);
	return newMesh;
}

Texture* Loader::loadTexture(const TextureData& data)
{
	if (!_context) {
		return nullptr;
	}
	_context->makeCurrent();

	auto textureImpl = _impl->createTexture();
	if (!textureImpl) {
		return nullptr;
	}

	Texture* newTexture = _textureAllocator.allocate();
	_textureAllocator.construct(newTexture, textureImpl);
	newTexture->init(data);
	_textures.push_back(newTexture);
	return newTexture;
}

Texture* Loader::createTexture(int width, int height)
{
	TextureData dummyData;
	dummyData.width = width;
	dummyData.height = height;
	dummyData.bytesPerPixel = 4;

	return loadTexture(dummyData);
}

Shader* Loader::loadShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	if (!_context) {
		return nullptr;
	}
	_context->makeCurrent();

	auto shaderImpl = _impl->createShader();
	if (!shaderImpl) {
		return nullptr;
	}

	Shader* newShader = _shaderAllocator.allocate();
	_shaderAllocator.construct(newShader, shaderImpl);
	newShader->init(vertexShader, fragmentShader);
	_shaders.push_back(newShader);
	return newShader;
}

void Loader::release(Mesh* mesh)
{
	if (!mesh) {
		return;
	}

	if (!_context) {
		return;
	}
	_context->makeCurrent();

	auto foundIt = std::find(_meshes.begin(), _meshes.end(), mesh);
	if (foundIt == _meshes.end()) {
		return;
	}

	_meshAllocator.destroy(mesh);
	_meshAllocator.deallocate(mesh);
	_meshes.erase(foundIt);
}

void Loader::release(Texture* texture)
{
	if (!texture) {
		return;
	}

	if (!_context) {
		return;
	}
	_context->makeCurrent();

	auto foundIt = std::find(_textures.begin(), _textures.end(), texture);
	if (foundIt == _textures.end()) {
		return;
	}

	_textureAllocator.destroy(texture);
	_textureAllocator.deallocate(texture);
	_textures.erase(foundIt);
}

void Loader::release(Shader* shader)
{
	if (!shader) {
		return;
	}

	if (!_context) {
		return;
	}
	_context->makeCurrent();

	auto foundIt = std::find(_shaders.begin(), _shaders.end(), shader);
	if (foundIt == _shaders.end()) {
		return;
	}

	_shaderAllocator.destroy(shader);
	_shaderAllocator.deallocate(shader);
	_shaders.erase(foundIt);
}
