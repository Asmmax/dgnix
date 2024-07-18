#pragma once
#include "BufferId.hpp"
#include <glm/vec3.hpp>

class ILoaderImpl;

class IGraphicsContext
{
public:
	IGraphicsContext() = default;
	IGraphicsContext(const IGraphicsContext& other) = delete;
	IGraphicsContext(IGraphicsContext&& other) = delete;
	IGraphicsContext& operator=(const IGraphicsContext& other) = delete;
	IGraphicsContext& operator=(IGraphicsContext&& other) = delete;

	virtual ~IGraphicsContext() = default;

	virtual void init() = 0;
	virtual void terminate() = 0;
	virtual void makeCurrent() = 0;
	virtual void getFrameSize(int& width, int& height) const = 0;

	virtual void setViewport(int width, int height) = 0;
	virtual void clear(const glm::vec3& background, bool withDepth = true) = 0;
	virtual void clearDepth() = 0;
	virtual void setupImgui() = 0;
	virtual void renderImgui() = 0;
	virtual void copyBuffer(BufferId srcId, int srcWidth, int srcHeight, BufferId dstId, int dstWidth, int dstHeightt) = 0;
	virtual void swapInterval(int frameCount) = 0;
	virtual void swapBuffers() = 0;

	virtual ILoaderImpl* createLoader() = 0;
};
