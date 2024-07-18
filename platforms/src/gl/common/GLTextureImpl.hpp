#pragma once
#include "resources/ITextureImpl.hpp"
#include "PoolAllocator.hpp"

class GLTextureImpl : public ITextureImpl
{
private:
	TextureId _textureHandle;
	PoolAllocator<GLTextureImpl>* _allocator;

public:
	explicit GLTextureImpl(PoolAllocator<GLTextureImpl>* allocator);

	void init() override;
	void terminate() override;
	void free() override;

	void applyTo(int texUnit) override;
	void clear(int texUnit) override;

	void updateData(const TextureData& data) override;
	void resize(int width, int height, int bytesPerPixel) override;

	TextureId getTextureHandle() const override { return _textureHandle; }

	TextureData getData(int width, int height, int bytesPerPixel) override;
};
