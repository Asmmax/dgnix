#pragma once
#include "BBox.hpp"

class IMeshImpl;
struct MeshData;

class Mesh
{
private:
	BBox _bounds;
	IMeshImpl* _impl;
	size_t _indicesCount;
	

public:
	Mesh(IMeshImpl* meshImpl);
	~Mesh();

	void init(const MeshData& data);
	void updateData(const MeshData& data);
	void draw();

	const BBox& getBoundingBox() const { return _bounds; }
};
