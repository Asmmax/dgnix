#include "Model.hpp"
#include "RenderQueue.hpp"
#include "objects/Batch.hpp"
#include <algorithm>

Model::Model(size_t poolSize /*= 100*/):
	_batchAllocator(poolSize)
{
	_batches.reserve(poolSize);
}

Model::~Model()
{
	for (auto& batch : _batches) {
		_batchAllocator.destroy(batch);
	}
}

void Model::render(RenderQueue& renderQueue, const glm::mat4& viewProjMatrix) const
{
	for (Batch* batch : _batches) {
		batch->render(renderQueue, viewProjMatrix);
	}
}

Batch* Model::createBatch()
{
	Batch* newBatch = _batchAllocator.allocate();
	_batchAllocator.construct(newBatch);

	const auto foundIt = std::lower_bound(_batches.begin(), _batches.end(), newBatch);
	_batches.insert(foundIt, newBatch);

	return newBatch;
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
