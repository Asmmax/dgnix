#pragma once
#include <vector>
#include <string>

struct TextureData
{
	int width = 0;
	int height = 0;
	int bytesPerPixel = 0;
	std::vector<unsigned char> data;
};

TextureData readTexture(const std::string& fileName);
bool writeTexture(const std::string& fileName, const TextureData& data);
