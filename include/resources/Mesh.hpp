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
	explicit Mesh(IMeshImpl* meshImpl);
	~Mesh();

	Mesh(const Mesh& other) = delete;
	Mesh(Mesh&& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;
	Mesh& operator=(Mesh&& other) = delete;

	void init(const MeshData& data);
	void updateData(const MeshData& data);
	void draw();

	const BBox& getBoundingBox() const { return _bounds; }
};
