#include "GLViewImpl.hpp"
#include "gl/gl_core_4_3.hpp"

GLViewImpl::GLViewImpl():
	_fboTextureId(0),
	_fboId(0),
	_fboDepthBufferId(0)
{
}

void GLViewImpl::init(TextureId fboTextureId)
{
	assert(fboTextureId);

	gl::GenFramebuffers(1, &_fboId.value);
	gl::BindFramebuffer(gl::FRAMEBUFFER, _fboId);

	_fboTextureId = fboTextureId;
	gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, _fboTextureId, 0);

	gl::GenRenderbuffers(1, &_fboDepthBufferId.value);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void GLViewImpl::terminate()
{
	gl::DeleteRenderbuffers(1, &_fboDepthBufferId.value);
	_fboDepthBufferId.value = 0;

	_fboTextureId.value = 0;

	gl::DeleteFramebuffers(1, &_fboId.value);
	_fboId.value = 0;
}

void GLViewImpl::beginDraw()
{
	gl::BindFramebuffer(gl::FRAMEBUFFER, _fboId);
}

void GLViewImpl::endDraw()
{
	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void GLViewImpl::resizeBuffer(int width, int height)
{
	gl::BindFramebuffer(gl::FRAMEBUFFER, _fboId);

	gl::BindRenderbuffer(gl::RENDERBUFFER, _fboDepthBufferId);
	gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT, width, height);
	gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, _fboDepthBufferId);

	assert(gl::CheckFramebufferStatus(gl::FRAMEBUFFER) == gl::FRAMEBUFFER_COMPLETE);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}
