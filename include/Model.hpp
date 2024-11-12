#pragma once
#include "DrawState.hpp"
#include "PoolAllocator.hpp"
#include <vector>
#include <glm/mat4x4.hpp>

class DirectLight;
class PointLight;
class Object;
class Batch;

class Model
{
private:
	std::vector<DirectLight*> _directLights;
	std::vector<PointLight*> _pointLights;
	std::vector<Batch*> _batches;
	DrawStateDef _state;

	PoolAllocator<DirectLight> _directLightAllocator;
	PoolAllocator<PointLight> _pointLightAllocator;
	PoolAllocator<Batch> _batchAllocator;

public:
	explicit Model(size_t poolSize = 100);
	~Model();

	Model(const Model& other) = delete;
	Model(Model&& other) = delete;
	Model& operator=(const Model& other) = delete;
	Model& operator=(Model&& other) = delete;

	void predraw(DrawStatePoolDef& statePool, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) const;
	void draw(DrawStatePoolDef& statePool) const;

	DrawStateDef& getState() { return _state; }
	const DrawStateDef& getState() const { return _state; }
	
	DirectLight* createDirectLight();
	PointLight* createPointLight();
	Batch* createBatch();

	void removeDirectLight(DirectLight* light);
	void removePointLight(PointLight* light);
	void removeBatch(Batch* batch);
};
