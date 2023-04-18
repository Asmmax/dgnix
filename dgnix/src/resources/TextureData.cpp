#include "resources/TextureData.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

TextureData loadTexture(const std::string& fileName)
{
	TextureData data;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* rawData = stbi_load(fileName.c_str(), &data.width, &data.height, &data.bytesPerPixel, 4);
	if (rawData == nullptr) {
		std::cerr << "Cannot open \"" << fileName << "\"\n";
		return data;
	}

	size_t dataSize = static_cast<size_t>(data.width) * data.height * data.bytesPerPixel;
	data.data.resize(dataSize);
	std::memcpy(&data.data[0], rawData, dataSize);

	stbi_image_free(rawData);

	return data;
}
