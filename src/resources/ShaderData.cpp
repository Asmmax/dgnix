#include "resources/ShaderData.hpp"
#include <fstream>
#include <iostream>

std::string loadShader(const std::string& fileName)
{
	std::string data;

	std::ifstream file;
	file.open(fileName);

	if (!file.is_open()) {
		std::cerr << "Shader file \"" << fileName << "\" could not be open!\n";
		return std::string();
	}

	data.clear();
	data.reserve(100);

	char c;
	while (!file.eof()) {
		file.get(c);
		data.push_back(c);
	}

	file.close();

	return data;
}
