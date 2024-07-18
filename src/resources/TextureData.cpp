#include "resources/TextureData.hpp"
#include <iostream>
#include <cstring>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

TextureData readTexture(const std::string& fileName)
{
	TextureData data;

	stbi_set_flip_vertically_on_load(true);

	unsigned char* rawData = stbi_load(fileName.c_str(), &data.width, &data.height, &data.bytesPerPixel, 0);
	if (rawData == nullptr) {
		std::cerr << "Cannot open \"" << fileName << "\"\n";
		return data;
	}

	size_t dataSize = static_cast<size_t>(data.width) * data.height * data.bytesPerPixel;
	data.data.resize(dataSize);
	std::memcpy(data.data.data(), rawData, dataSize);

	stbi_image_free(rawData);

	return data;
}

bool writeTexture(const std::string& fileName, const TextureData& data)
{
	assert(!data.data.empty());

	std::vector<unsigned char> flippedData = data.data;

	const size_t rowSize = static_cast<size_t>(data.width) * data.bytesPerPixel;
	std::vector<unsigned char> rowBuffer(rowSize);
	for (int y = 0; y < data.height / 2; y++) {
		unsigned char* rowTop = flippedData.data() + y * rowSize;
		unsigned char* rowBottom = flippedData.data() + (static_cast<size_t>(data.height) - y - 1) * rowSize;

		std::memcpy(rowBuffer.data(), rowTop, rowSize);
		std::memcpy(rowTop, rowBottom, rowSize);
		std::memcpy(rowBottom, rowBuffer.data(), rowSize);
	}

	const std::string ext = fileName.substr(fileName.find_last_of('.'));
	
	if (ext == ".bmp") {
		return stbi_write_bmp(fileName.c_str(), data.width, data.height, data.bytesPerPixel, flippedData.data()) == 1;
	}
	else if (ext == ".jpg") {
		return stbi_write_jpg(fileName.c_str(), data.width, data.height, data.bytesPerPixel, flippedData.data(), 75) == 1;
	}
	else if (ext == ".tga") {
		return stbi_write_tga(fileName.c_str(), data.width, data.height, data.bytesPerPixel, flippedData.data()) == 1;
	}
	else if (ext == ".png") {
		return stbi_write_png(fileName.c_str(), data.width, data.height, data.bytesPerPixel, flippedData.data(), static_cast<int>(rowSize)) == 1;
	}

	return false;
}
