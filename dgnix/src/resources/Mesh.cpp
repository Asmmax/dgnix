#include "resources/Mesh.hpp"
#include "resources/IMeshImpl.hpp"
#include "resources/MeshData.hpp"

Mesh::Mesh(IMeshImpl* meshImpl):
	_impl(meshImpl),
	_indicesCount(0)
{
}

Mesh::~Mesh()
{
	_impl->terminate();
	_impl->free();
}

void Mesh::init(const MeshData& data)
{
	_impl->init();
	updateData(data);
}

void Mesh::updateData(const MeshData& data)
{
	_impl->updateData(data);
	_indicesCount = data.indices.size();

	_bounds = BBox();
	for (auto& point : data.positions) {
		_bounds.addPoint(point.x, point.y, point.z);
	}
}

void Mesh::draw()
{
	_impl->draw(_indicesCount);
}
