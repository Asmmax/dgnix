#pragma once
#include "DrawState.hpp"
#include "PoolAllocator.hpp"
#include <vector>

class Shader;
class Texture;
class Object;

class Batch
{
private:
	DrawStateDef _state;
	Shader* _shader;
	std::vector<std::pair<StringId, Texture*>> _textures;
	std::vector<Object*> _objects;
	std::vector<Object*> _hiddenObjects;
	std::vector<Object*> _culledObjects;

	PoolAllocator<Object> _objectAllocator;

public:
	Batch(size_t poolSize = 100);
	~Batch();

	DrawStateDef& getState() { return _state; }
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

	void draw(DrawStatePoolDef& statePool);
};
