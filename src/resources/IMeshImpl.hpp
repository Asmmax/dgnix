#pragma once
#include <cstddef>

struct MeshData;

class IMeshImpl
{
public:
	IMeshImpl() = default;
	IMeshImpl(const IMeshImpl& other) = delete;
	IMeshImpl(IMeshImpl&& other) = delete;
	IMeshImpl& operator=(const IMeshImpl& other) = delete;
	IMeshImpl& operator=(IMeshImpl&& other) = delete;

	virtual void init() = 0;
	virtual void terminate() = 0;
	virtual void free() = 0;

	virtual void draw(size_t indicesCount) = 0;

	virtual void updateData(const MeshData& data) = 0;

protected:
	virtual ~IMeshImpl() = default;
};
