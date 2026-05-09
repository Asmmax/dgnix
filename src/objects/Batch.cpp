#include "objects/Batch.hpp"

#include "RenderQueue.hpp"
#include "Material.hpp"
#include "objects/Object.hpp"
#include "resources/Shader.hpp"
#include "resources/Texture.hpp"
#include <algorithm>

Batch::Batch(size_t poolSize /*= 100*/) :
	_shader(nullptr),
	_objectAllocator(poolSize)
{
	_objects.reserve(poolSize);
}

Batch::~Batch()
{
	for (auto& object : _objects) {
		_objectAllocator.destroy(object);
	}
}

DrawStateDef& Batch::getState()
{
	_dirtyMaterialData = true;
	return _state;
}

void Batch::setShader(Shader* shader)
{
	_shader = shader;
}

bool Batch::hasTexture(const StringId& name) const
{
	const auto it = std::find_if(_textures.begin(), _textures.end(), [name](auto& texturePair) {
		return texturePair.first == name;
		});

	return it != _textures.end();
}

void Batch::addTexture(const StringId& name, Texture* texture)
{
	if (!texture) {
		return;
	}

	assert(!hasTexture(name));

	_textures.emplace_back(name, texture);
	_material.setTexture(name, texture);
}

void Batch::setTexture(const StringId& name, Texture* texture)
{
	if (!texture) {
		return;
	}

	assert(hasTexture(name));
	const auto it = std::find_if(_textures.begin(), _textures.end(), [name](auto& texturePair) {
		return texturePair.first == name;
		});

	it->second = texture;
	_material.setTexture(name, texture);
}

void Batch::removeTexture(const StringId& name)
{
	assert(hasTexture(name));
	const auto it = std::find_if(_textures.begin(), _textures.end(), [name](auto& texturePair) {
		return texturePair.first == name;
		});
	_textures.erase(it);
	_material.removeTexture(name);
}

Object* Batch::createObject()
{
	Object* newObject = _objectAllocator.allocate();
	_objectAllocator.construct(newObject);

	const auto foundIt = std::lower_bound(_objects.begin(), _objects.end(), newObject);
	_objects.insert(foundIt, newObject);

	return newObject;
}

Object* Batch::createObject(Object* other)
{
	Object* newObject = _objectAllocator.allocate();
	_objectAllocator.construct(newObject, *other);

	const auto foundIt = std::lower_bound(_objects.begin(), _objects.end(), newObject);
	_objects.insert(foundIt, newObject);

	return newObject;
}

void Batch::enableObject(Object* object)
{
	const auto foundHidObjIt = std::lower_bound(_hiddenObjects.begin(), _hiddenObjects.end(), object);
	if (*foundHidObjIt != object) {
		return;
	}

	_hiddenObjects.erase(foundHidObjIt);

	const auto foundObjIt = std::lower_bound(_objects.begin(), _objects.end(), object);
	_objects.insert(foundObjIt, object);
}

void Batch::disableObject(Object* object)
{
	const auto foundObjIt = std::lower_bound(_objects.begin(), _objects.end(), object);
	if (*foundObjIt != object) {
		return;
	}

	_objects.erase(foundObjIt);

	const auto foundHidObjIt = std::lower_bound(_hiddenObjects.begin(), _hiddenObjects.end(), object);
	_hiddenObjects.insert(foundHidObjIt, object);
}

void Batch::removeObject(Object* object)
{
	const auto foundIt = std::lower_bound(_objects.begin(), _objects.end(), object);
	if (*foundIt != object) {
		return;
	}

	_objects.erase(foundIt);
	_objectAllocator.destroy(object);
	_objectAllocator.deallocate(object);
}

void Batch::clear()
{
	for (Object* object : _objects) {
		_objectAllocator.destroy(object);
	}
	_objectAllocator.reset();
	_objects.clear();
}

void Batch::render(RenderQueue& renderQueue, const glm::mat4& viewProjMatrix)
{
	if (_objects.empty()) {
		return;
	}

	_culledObjects.clear();
	for (Object* object : _objects)
	{
		if (object->isCaughtIntoView(viewProjMatrix)) {
			_culledObjects.push_back(object);
		}
	}

	if (_culledObjects.empty()) {
		return;
	}

	if (_dirtyMaterialData) {
		_material.getMaterialData().clear();
		_state.fill(_material.getMaterialData());
		_dirtyMaterialData = false;
	}

	for (Object* object : _culledObjects)
	{
		object->render(renderQueue, _shader, &_material);
	}
}
