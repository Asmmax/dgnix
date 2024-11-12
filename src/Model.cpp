#include "Model.hpp"
#include "objects/Batch.hpp"
#include "objects/DirectLight.hpp"
#include "objects/PointLight.hpp"
#include <algorithm>

Model::Model(size_t poolSize /*= 100*/):
	_directLightAllocator(poolSize),
	_pointLightAllocator(poolSize),
	_batchAllocator(poolSize)
{
	_directLights.reserve(poolSize);
	_pointLights.reserve(poolSize);
	_batches.reserve(poolSize);
}

Model::~Model()
{
	for (auto& light : _directLights) {
		_directLightAllocator.destroy(light);
	}

	for (auto& light : _pointLights) {
		_pointLightAllocator.destroy(light);
	}

	for (auto& batch : _batches) {
		_batchAllocator.destroy(batch);
	}
}

void Model::predraw(DrawStatePoolDef& statePool, const glm::mat4& viewMatrix, const glm::mat4& projMatrix) const
{
	const glm::mat4 viewProjMat = projMatrix * viewMatrix;

	auto& currentState = statePool.get();
	static const StringId viewMatrixName = StringId("ViewMatrix");
	currentState.add(viewMatrixName, viewMatrix);
	static const StringId projMatrixName = StringId("ProjectionMatrix");
	currentState.add(projMatrixName, projMatrix);
	static const StringId viewProjMatrixName = StringId("ViewProjectionMatrix");
	currentState.add(viewProjMatrixName, viewProjMat);

	static const StringId directLightCountName = StringId("DirectLightCount");
	currentState.addOrSet(directLightCountName, 0);

	for (DirectLight* light : _directLights) {
		light->predraw(statePool);
	}

	static const StringId pointLightCountName = StringId("PointLightCount");
	currentState.addOrSet(pointLightCountName, 0);

	for (PointLight* light : _pointLights) {
		light->predraw(statePool);
	}

	_state.apply(currentState);
}

void Model::draw(DrawStatePoolDef& statePool) const
{
	for (Batch* batch : _batches) {
		statePool.push();
		batch->draw(statePool);
		statePool.pop();
	}
}

DirectLight* Model::createDirectLight()
{
	DirectLight* newLight = _directLightAllocator.allocate();
	_directLightAllocator.construct(newLight);

	const auto foundIt = std::lower_bound(_directLights.begin(), _directLights.end(), newLight);
	_directLights.insert(foundIt, newLight);

	return newLight;
}

PointLight* Model::createPointLight()
{
	PointLight* newLight = _pointLightAllocator.allocate();
	_pointLightAllocator.construct(newLight);

	const auto foundIt = std::lower_bound(_pointLights.begin(), _pointLights.end(), newLight);
	_pointLights.insert(foundIt, newLight);

	return newLight;
}

Batch* Model::createBatch()
{
	Batch* newBatch = _batchAllocator.allocate();
	_batchAllocator.construct(newBatch);

	const auto foundIt = std::lower_bound(_batches.begin(), _batches.end(), newBatch);
	_batches.insert(foundIt, newBatch);

	return newBatch;
}

void Model::removeDirectLight(DirectLight* light)
{
	const auto foundIt = std::lower_bound(_directLights.begin(), _directLights.end(), light);
	if (*foundIt != light) {
		return;
	}

	_directLights.erase(foundIt);
	_directLightAllocator.destroy(light);
	_directLightAllocator.deallocate(light);
}

void Model::removePointLight(PointLight* light)
{
	const auto foundIt = std::lower_bound(_pointLights.begin(), _pointLights.end(), light);
	if (*foundIt != light) {
		return;
	}

	_pointLights.erase(foundIt);
	_pointLightAllocator.destroy(light);
	_pointLightAllocator.deallocate(light);
}

void Model::removeBatch(Batch* batch)
{
	const auto foundIt = std::lower_bound(_batches.begin(), _batches.end(), batch);
	if (*foundIt != batch) {
		return;
	}

	_batches.erase(foundIt);
	_batchAllocator.destroy(batch);
	_batchAllocator.deallocate(batch);
}
