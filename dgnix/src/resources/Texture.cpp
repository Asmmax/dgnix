#include "resources/Texture.hpp"
#include "resources/ITextureImpl.hpp"
#include "resources/TextureData.hpp"

Texture::Texture(ITextureImpl* textureImpl):
	_impl(textureImpl),
	_textureUnit(-1),
	_width(0),
	_height(0)
{
}

Texture::~Texture()
{
	_impl->terminate();
	_impl->free();
}

void Texture::init(const TextureData& data)
{
	_impl->init();
	updateData(data);
}

void Texture::apply(int textureUnit)
{
	_impl->applyTo(textureUnit);
	_textureUnit = textureUnit;
}

void Texture::clear()
{
	if (_textureUnit < 0) {
		return;
	}
	_impl->clear(_textureUnit);
	_textureUnit = -1;
}

void Texture::updateData(const TextureData& data)
{
	_impl->updateData(data);
	_width = data.width;
	_height = data.height;
}

void Texture::resize(int width, int height)
{
	if (_width == width && _height == height) {
		return;
	}

	_impl->resize(width, height);
	_width = width;
	_height = height;
}

TextureId Texture::getId() const
{
	return static_cast<TextureId>(_impl->getTextureHandle());
}
