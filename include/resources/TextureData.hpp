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

TextureData loadTexture(const std::string& fileName);
