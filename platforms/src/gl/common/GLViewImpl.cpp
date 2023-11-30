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
	gl::GenFramebuffers(1, &_fboId);
	gl::BindFramebuffer(gl::FRAMEBUFFER, _fboId);

	_fboTextureId = static_cast<GLuint>(fboTextureId);
	gl::BindTexture(gl::TEXTURE_2D, _fboTextureId);

	gl::GenRenderbuffers(1, &_fboDepthBufferId);
	gl::BindRenderbuffer(gl::RENDERBUFFER, _fboDepthBufferId);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void GLViewImpl::terminate()
{
	gl::DeleteRenderbuffers(1, &_fboDepthBufferId);
	_fboDepthBufferId = 0;

	_fboTextureId = 0;

	gl::DeleteFramebuffers(1, &_fboId);
	_fboId = 0;
}

void GLViewImpl::beginDraw(int width, int height, const glm::vec3& background)
{
	gl::BindFramebuffer(gl::FRAMEBUFFER, _fboId);
	gl::Viewport(0, 0, width, height);

	gl::ClearColor(background.r, background.g, background.b, 1.0f);
	gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);
}

void GLViewImpl::endDraw()
{
	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}

void GLViewImpl::resizeBuffer(int width, int height)
{
	gl::BindFramebuffer(gl::FRAMEBUFFER, _fboId);

	gl::BindTexture(gl::TEXTURE_2D, _fboTextureId);
	gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, nullptr);
	gl::FramebufferTexture2D(gl::FRAMEBUFFER, gl::COLOR_ATTACHMENT0, gl::TEXTURE_2D, _fboTextureId, 0);

	gl::BindRenderbuffer(gl::RENDERBUFFER, _fboDepthBufferId);
	gl::RenderbufferStorage(gl::RENDERBUFFER, gl::DEPTH_COMPONENT, width, height);
	gl::FramebufferRenderbuffer(gl::FRAMEBUFFER, gl::DEPTH_ATTACHMENT, gl::RENDERBUFFER, _fboDepthBufferId);

	gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
}
