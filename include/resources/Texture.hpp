#pragma once
#include "TextureId.hpp"

class ITextureImpl;
struct TextureData;

class Texture
{
private:
	ITextureImpl* _impl;
	int _textureUnit;
	int _width;
	int _height;

public:
	explicit Texture(ITextureImpl* textureImpl);
	~Texture();

	Texture(const Texture& other) = delete;
	Texture(Texture&& other) = delete;
	Texture& operator=(const Texture& other) = delete;
	Texture& operator=(Texture&& other) = delete;

	void init(const TextureData& data);
	void apply(int textureUnit);
	void clear();

	void updateData(const TextureData& data);
	void resize(int width, int height);

	TextureId getId() const;
	int getWidth() const { return _width; }
	int getHeight() const { return _height; }
};
