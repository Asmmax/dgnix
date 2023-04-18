#include "GLTextureImpl.hpp"
#include "resources/TextureData.hpp"
#include "gl/gl_core_4_3.hpp"

GLTextureImpl::GLTextureImpl(PoolAllocator<GLTextureImpl>* allocator):
	_textureHandle(0),
	_allocator(allocator)
{
}

void GLTextureImpl::init()
{
	gl::GenTextures(1, &_textureHandle);

	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);

	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void GLTextureImpl::terminate()
{
	gl::DeleteTextures(1, &_textureHandle);
	_textureHandle = 0;
}

void GLTextureImpl::free()
{
	PoolAllocator<GLTextureImpl>* tempAllocator = _allocator;
	tempAllocator->destroy(this);
	tempAllocator->deallocate(this);
}

void GLTextureImpl::applyTo(int texUnit)
{
	gl::ActiveTexture(gl::TEXTURE0 + texUnit);
	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
}

void GLTextureImpl::clear(int texUnit)
{
	gl::ActiveTexture(gl::TEXTURE0 + texUnit);
	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void GLTextureImpl::updateData(const TextureData& data)
{
	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
	gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, data.width, data.height, 0, gl::RGBA, gl::UNSIGNED_BYTE, data.data.empty() ? nullptr : data.data.data());
}

void GLTextureImpl::resize(int width, int height)
{
	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
	gl::TexImage2D(gl::TEXTURE_2D, 0, gl::RGBA, width, height, 0, gl::RGBA, gl::UNSIGNED_BYTE, nullptr);
}
