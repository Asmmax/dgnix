#pragma once
#include "Material.hpp"
#include "DrawState.hpp"
#include "PoolAllocator.hpp"
#include <vector>

class Shader;
class Texture;
class Object;
class RenderQueue;

class Batch
{
private:
	DrawStateDef _state;
	Shader* _shader;
	std::vector<std::pair<StringId, Texture*>> _textures;
	std::vector<Object*> _objects;
	std::vector<Object*> _hiddenObjects;
	std::vector<Object*> _culledObjects;
	Material _material;
	bool _dirtyMaterialData = false;

	PoolAllocator<Object> _objectAllocator;

public:
	explicit Batch(size_t poolSize = 100);
	~Batch();

	Batch(const Batch& other) = delete;
	Batch(Batch&& other) = delete;
	Batch& operator=(const Batch& other) = delete;
	Batch& operator=(Batch&& other) = delete;

	DrawStateDef& getState();
	const DrawStateDef& getState() const { return _state; }

	void setShader(Shader* shader);

	bool hasTexture(const StringId& name) const;
	void addTexture(const StringId& name, Texture* texture);
	void setTexture(const StringId& name, Texture* texture);
	void removeTexture(const StringId& name);

	Object* createObject();
	Object* createObject(Object* other);
	void enableObject(Object* object);
	void disableObject(Object* object);
	void removeObject(Object* object);
	void clear();

	void render(RenderQueue& renderQueue, const glm::mat4& viewProjMatrix);
};
