#pragma once

struct MeshData;

class IMeshImpl
{
public:
	virtual void init() = 0;
	virtual void terminate() = 0;
	virtual void free() = 0;

	virtual void draw(size_t indicesCount) = 0;

	virtual void updateData(const MeshData& data) = 0;

protected:
	virtual ~IMeshImpl() {}
};
