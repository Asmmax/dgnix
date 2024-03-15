#pragma once
#include "resources/TextureId.hpp"
#include <glm/vec3.hpp>

class IViewImpl
{
public:
	IViewImpl() = default;
	IViewImpl(const IViewImpl& other) = delete;
	IViewImpl(IViewImpl&& other) = delete;
	IViewImpl& operator=(const IViewImpl& other) = delete;
	IViewImpl& operator=(IViewImpl&& other) = delete;

	virtual ~IViewImpl() = default;

	virtual void init(TextureId fboTextureId) = 0;
	virtual void terminate() = 0;

	virtual void beginDraw(int width, int height, const glm::vec3& background) = 0;
	virtual void endDraw() = 0;
	virtual void resizeBuffer(int width, int height) = 0;
};
