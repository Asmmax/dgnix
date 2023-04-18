#pragma once
#include "DrawState.hpp"
#include "PoolAllocator.hpp"
#include <vector>
#include <glm/mat4x4.hpp>

class Light;
class Object;
class Batch;

class Model
{
private:
	std::vector<Light*> _lights;
	std::vector<Batch*> _batches;
	DrawStateDef _state;

	PoolAllocator<Light> _lightAllocator;
	PoolAllocator<Batch> _batchAllocator;

public:
	Model(size_t poolSize = 100);
	~Model();

	void predraw(DrawStatePoolDef& statePool, const glm::mat4& viewMatrix, const glm::mat4& projMatrix);
	void draw(DrawStatePoolDef& statePool);

	DrawStateDef& getState() { return _state; }
	const DrawStateDef& getState() const { return _state; }
	
	Light* createLight();
	Batch* createBatch();

	void removeLight(Light* light);
	void removeBatch(Batch* batch);
};
