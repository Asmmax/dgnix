#pragma once
#include "IViewImpl.hpp"

class GLViewImpl : public IViewImpl
{
private:
	TextureId _fboTextureId;
	BufferId _fboId;
	BufferId _fboDepthBufferId;

public:
	GLViewImpl();

	void init(TextureId fboTextureId) override;
	void terminate() override;

	void beginDraw() override;
	void endDraw() override;
	void resizeBuffer(int width, int height) override;
	BufferId getFrameBufferId() const { return _fboId; }
};
