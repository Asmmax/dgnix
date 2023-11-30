#pragma once
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"
#include "resources/Shader.hpp"
#include "PoolAllocator.hpp"
#include <vector>
#include <string>

class ILoaderImpl;
struct MeshData;
struct TextureData;

class IGraphicsContext;

class Loader
{
private:
	ILoaderImpl* _impl;
	IGraphicsContext* _context;

	std::vector<Mesh*> _meshes;
	std::vector<Texture*> _textures;
	std::vector<Shader*> _shaders;

	PoolAllocator<Mesh> _meshAllocator;
	PoolAllocator<Texture> _textureAllocator;
	PoolAllocator<Shader> _shaderAllocator;

public:
	Loader(ILoaderImpl* loaderImpl, size_t poolSize = 100);
	~Loader();

	void init(IGraphicsContext* context);

	Mesh* loadMesh(const MeshData& data);
	Texture* loadTexture(const TextureData& data);
	Texture* createTexture(int width, int height);
	Shader* loadShader(const std::string& vertexShader, const std::string& fragmentShader);

	void release(Mesh* mesh);
	void release(Texture* texture);
	void release(Shader* shader);
};
