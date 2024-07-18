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
	gl::GenTextures(1, &_textureHandle.value);

	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::LINEAR);
	gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::LINEAR);

	gl::BindTexture(gl::TEXTURE_2D, 0);
}

void GLTextureImpl::terminate()
{
	gl::DeleteTextures(1, &_textureHandle.value);
	_textureHandle.value = 0;
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

static GLint getGlInternalFormat(int bytesPerPixel)
{
	switch (bytesPerPixel) {
	case 1:
		return gl::R8;
	case 2:
		return gl::RG8;
	case 3:
		return gl::RGB;
	case 4:
		return gl::RGBA;
	}
	return 0;
}

static GLint getGlFormat(int bytesPerPixel)
{
	switch (bytesPerPixel) {
	case 1:
		return gl::RED;
	case 2:
		return gl::RG;
	case 3:
		return gl::RGB;
	case 4:
		return gl::RGBA;
	}
	return 0;
}

void GLTextureImpl::updateData(const TextureData& data)
{
	const GLint internalFormat = getGlInternalFormat(data.bytesPerPixel);
	const GLint format = getGlFormat(data.bytesPerPixel);
	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
	gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, data.width, data.height, 0, format, gl::UNSIGNED_BYTE, data.data.empty() ? nullptr : data.data.data());
}

void GLTextureImpl::resize(int width, int height, int bytesPerPixel)
{
	const GLint internalFormat = getGlInternalFormat(bytesPerPixel);
	const GLint format = getGlFormat(bytesPerPixel);
	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);
	gl::TexImage2D(gl::TEXTURE_2D, 0, internalFormat, width, height, 0, format, gl::UNSIGNED_BYTE, nullptr);
}

TextureData GLTextureImpl::getData(int width, int height, int bytesPerPixel)
{
	TextureData data;
	data.width = width;
	data.height = height;
	data.bytesPerPixel = bytesPerPixel;
	const size_t dataSize = static_cast<size_t>(width) * static_cast<size_t>(height) * static_cast<size_t>(bytesPerPixel);
	data.data.resize(dataSize);

	gl::BindTexture(gl::TEXTURE_2D, _textureHandle);

	const GLint format = getGlFormat(bytesPerPixel);
	gl::GetTexImage(gl::TEXTURE_2D, 0, format, gl::UNSIGNED_BYTE, data.data.data());

	return data;
}
