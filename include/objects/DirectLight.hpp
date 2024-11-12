#pragma once
#include "DrawState.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>

class DirectLight
{
private:
	class LightNames
	{
	private:
		int _id;
		StringId _colorName;
		StringId _intensityName;

	public:
		explicit LightNames(int id);

		int getId() const { return _id; }
		const StringId& getColorName() const { return _colorName; }
		const StringId& getIntensityName() const { return _intensityName; }

		static std::vector<StringId>& getLightDirNames();
		static std::vector<StringId> initLightDirNames();
	};

private:
	glm::vec3 _direction;
	glm::vec3 _color;
	float _intensity;

	std::unique_ptr<LightNames> _names;

public:
	DirectLight();

	void setColor(const glm::vec3& color);
	void setIntensity(float intensity);
	void setDirection(const glm::vec3& dir);

	void predraw(DrawStatePoolDef& statePool);

	static int MAX_LIGHT_COUNT;

private:
	static int nextFreeId(const DrawStateDef& state);
};