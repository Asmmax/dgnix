#pragma once
#include "IViewImpl.hpp"

class GLViewImpl : public IViewImpl
{
private:
	unsigned int _fboTextureId;
	unsigned int _fboId;
	unsigned int _fboDepthBufferId;

public:
	GLViewImpl();

	void init(TextureId fboTextureId) override;
	void terminate() override;

	void beginDraw(int width, int height, const glm::vec3& background) override;
	void endDraw() override;
	void resizeBuffer(int width, int height) override;
};
