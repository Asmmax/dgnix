#include "Model.hpp"
#include "objects/Batch.hpp"
#include "objects/Light.hpp"
#include <algorithm>

Model::Model(size_t poolSize /*= 100*/):
	_lightAllocator(poolSize),
	_batchAllocator(poolSize)
{
	_lights.reserve(poolSize);
	_batches.reserve(poolSize);
}

Model::~Model()
{
	for (auto& light : _lights) {
		_lightAllocator.destroy(light);
	}

	for (auto& batch : _batches) {
		_batchAllocator.destroy(batch);
	}
}

void Model::predraw(DrawStatePoolDef& statePool, const glm::mat4& viewMatrix, const glm::mat4& projMatrix)
{
	glm::mat4 viewProjMat = projMatrix * viewMatrix;

	auto& currentState = statePool.get();
	static StringId viewMatrixName = StringId("ViewMatrix");
	currentState.add(viewMatrixName, viewMatrix);
	static StringId projMatrixName = StringId("ProjectionMatrix");
	currentState.add(projMatrixName, projMatrix);
	static StringId viewProjMatrixName = StringId("ViewProjectionMatrix");
	currentState.add(viewProjMatrixName, viewProjMat);

	for (auto& light : _lights) {
		light->predraw(statePool);
	}

	_state.apply(currentState);
}

void Model::draw(DrawStatePoolDef& statePool)
{
	for (auto& batch : _batches) {
		statePool.push();
		batch->draw(statePool);
		statePool.pop();
	}
}

Light* Model::createLight()
{
	Light* newLight = _lightAllocator.allocate();
	_lightAllocator.construct(newLight);

	auto foundIt = std::lower_bound(_lights.begin(), _lights.end(), newLight);
	_lights.insert(foundIt, newLight);

	return newLight;
}

Batch* Model::createBatch()
{
	Batch* newBatch = _batchAllocator.allocate();
	_batchAllocator.construct(newBatch);

	auto foundIt = std::lower_bound(_batches.begin(), _batches.end(), newBatch);
	_batches.insert(foundIt, newBatch);

	return newBatch;
}

void Model::removeLight(Light* light)
{
	auto foundIt = std::lower_bound(_lights.begin(), _lights.end(), light);
	if (*foundIt != light) {
		return;
	}

	_lights.erase(foundIt);
	_lightAllocator.destroy(light);
	_lightAllocator.deallocate(light);
}

void Model::removeBatch(Batch* batch)
{
	auto foundIt = std::lower_bound(_batches.begin(), _batches.end(), batch);
	if (*foundIt != batch) {
		return;
	}

	_batches.erase(foundIt);
	_batchAllocator.destroy(batch);
	_batchAllocator.deallocate(batch);
}
