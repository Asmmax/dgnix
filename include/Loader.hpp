#pragma once
#include "resources/Mesh.hpp"
#include "resources/Texture.hpp"
#include "resources/Shader.hpp"
#include "Material.hpp"
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
	std::vector<Material*> _materials;

	PoolAllocator<Mesh> _meshAllocator;
	PoolAllocator<Texture> _textureAllocator;
	PoolAllocator<Shader> _shaderAllocator;
	PoolAllocator<Material> _materialAllocator;

public:
	explicit Loader(ILoaderImpl* loaderImpl, size_t poolSize = 100);
	~Loader();

	Loader(const Loader& other) = delete;
	Loader(Loader&& other) = delete;
	Loader& operator=(const Loader& other) = delete;
	Loader& operator=(Loader&& other) = delete;

	void init(IGraphicsContext* context);

	Mesh* loadMesh(const MeshData& data);
	Texture* loadTexture(const TextureData& data);
	Texture* createTexture(int width, int height);
	Shader* loadShader(const std::string& vertexShader, const std::string& fragmentShader);
	Material* createMaterial();

	void release(Mesh* mesh);
	void release(Texture* texture);
	void release(Shader* shader);
	void release(Material* material);
};
