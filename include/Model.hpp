#pragma once
#include "DrawState.hpp"
#include "PoolAllocator.hpp"
#include <vector>
#include <glm/mat4x4.hpp>

class Object;
class Batch;

class Model
{
private:
	std::vector<Batch*> _batches;
	DrawStateDef _state;

	PoolAllocator<Batch> _batchAllocator;

public:
	explicit Model(size_t poolSize = 100);
	~Model();

	Model(const Model& other) = delete;
	Model(Model&& other) = delete;
	Model& operator=(const Model& other) = delete;
	Model& operator=(Model&& other) = delete;

	void draw(DrawStatePoolDef& statePool) const;

	DrawStateDef& getState() { return _state; }
	const DrawStateDef& getState() const { return _state; }
	
	Batch* createBatch();
	void removeBatch(Batch* batch);
};
