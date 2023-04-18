#include "objects/Batch.hpp"
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

void Batch::setShader(Shader* shader)
{
	_shader = shader;
}

bool Batch::hasTexture(const StringId& name) const
{
	auto&& it = std::find_if(_textures.begin(), _textures.end(), [name](auto& texturePair) {
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
}

void Batch::setTexture(const StringId& name, Texture* texture)
{
	if (!texture) {
		return;
	}

	assert(hasTexture(name));
	auto&& it = std::find_if(_textures.begin(), _textures.end(), [name](auto& texturePair) {
		return texturePair.first == name;
		});

	it->second = texture;
}

void Batch::removeTexture(const StringId& name)
{
	assert(hasTexture(name));
	auto&& it = std::find_if(_textures.begin(), _textures.end(), [name](auto& texturePair) {
		return texturePair.first == name;
		});
	_textures.erase(it);
}

Object* Batch::createObject()
{
	Object* newObject = _objectAllocator.allocate();
	_objectAllocator.construct(newObject);

	auto foundIt = std::lower_bound(_objects.begin(), _objects.end(), newObject);
	_objects.insert(foundIt, newObject);

	return newObject;
}

Object* Batch::createObject(Object* other)
{
	Object* newObject = _objectAllocator.allocate();
	_objectAllocator.construct(newObject, *other);

	auto foundIt = std::lower_bound(_objects.begin(), _objects.end(), newObject);
	_objects.insert(foundIt, newObject);

	return newObject;
}

void Batch::enableObject(Object* object)
{
	auto foundHidObjIt = std::lower_bound(_hiddenObjects.begin(), _hiddenObjects.end(), object);
	if (*foundHidObjIt != object) {
		return;
	}

	_hiddenObjects.erase(foundHidObjIt);

	auto foundObjIt = std::lower_bound(_objects.begin(), _objects.end(), object);
	_objects.insert(foundObjIt, object);
}

void Batch::disableObject(Object* object)
{
	auto foundObjIt = std::lower_bound(_objects.begin(), _objects.end(), object);
	if (*foundObjIt != object) {
		return;
	}

	_objects.erase(foundObjIt);

	auto foundHidObjIt = std::lower_bound(_hiddenObjects.begin(), _hiddenObjects.end(), object);
	_hiddenObjects.insert(foundHidObjIt, object);
}

void Batch::removeObject(Object* object)
{
	auto foundIt = std::lower_bound(_objects.begin(), _objects.end(), object);
	if (*foundIt != object) {
		return;
	}

	_objects.erase(foundIt);
	_objectAllocator.destroy(object);
	_objectAllocator.deallocate(object);
}

void Batch::clear()
{
	for (auto& object : _objects) {
		_objectAllocator.destroy(object);
	}
	_objectAllocator.reset();
	_objects.clear();
}

void Batch::draw(DrawStatePoolDef& statePool)
{
	if (_objects.empty()) {
		return;
	}

	auto& state = statePool.get();

	static StringId viewProjMatrixName = StringId("ViewProjectionMatrix");
	auto& viewProjMatrix = state.get<glm::mat4>(viewProjMatrixName);

	_culledObjects.clear();
	for (auto& object : _objects)
	{
		if (object->isCaughtIntoView(viewProjMatrix)) {
			_culledObjects.push_back(object);
		}
	}

	if (_culledObjects.empty()) {
		return;
	}

	_shader->use();

	state.apply(*_shader);

	int textureUnit = 0;
	for (auto& texture : _textures) {
		auto location = _shader->getLocation(texture.first);
		if (location == -1) {
			continue;
		}

		texture.second->apply(textureUnit);
		_shader->setUniform(location, textureUnit);
		textureUnit++;
	}

	_state.apply(*_shader);

	static StringId viewMatrixName = StringId("ViewMatrix");
	auto& viewMatrix = state.get<glm::mat4>(viewMatrixName);

	static StringId projMatrixName = StringId("ProjectionMatrix");
	auto& projMatrix = state.get<glm::mat4>(projMatrixName);

	for (auto& object : _culledObjects)
	{
		object->draw(_shader, viewMatrix, projMatrix);
	}

	_shader->clear();
}
