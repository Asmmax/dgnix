#pragma once

struct TextureData;

class ITextureImpl
{
public:
	virtual void init() = 0;
	virtual void terminate() = 0;
	virtual void free() = 0;

	virtual void applyTo(int texUnit) = 0;
	virtual void clear(int texUnit) = 0;

	virtual void updateData(const TextureData& data) = 0;
	virtual void resize(int width, int height) = 0;

	virtual unsigned int getTextureHandle() const = 0;

protected:
	virtual ~ITextureImpl() {}
};
