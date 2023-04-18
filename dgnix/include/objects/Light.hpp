#pragma once
#include "DrawState.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec3.hpp>
#include <memory>

class Light
{
private:
	class LightNames
	{
	private:
		int _id;
		StringId _colorName;
		StringId _intensityName;
		StringId _radiusName;
		StringId _fadingAreaName;

	public:
		LightNames(int id);

		int getId() const { return _id; }
		const StringId& getColorName() const { return _colorName; }
		const StringId& getIntensityName() const { return _intensityName; }
		const StringId& getRadiusName() const { return _radiusName; }
		const StringId& getFadingAreaName() const { return _fadingAreaName; }

		static std::vector<StringId>& getLightPosNames();
		static std::vector<StringId> initLightPosNames();
	};

private:
	glm::vec3 _position;
	glm::vec3 _color;
	float _intensity;
	float _radius;
	float _fadingArea;

	std::unique_ptr<LightNames> _names;

public:
	Light();

	void setColor(const glm::vec3& color);
	void setIntensity(float intensity);
	void setRadius(float radius);
	void setFadingArea(float fadingArea);
	void setPosition(const glm::vec3& position);

	void predraw(DrawStatePoolDef& statePool);

	static int MAX_LIGHT_COUNT;

private:
	int nextFreeId(const DrawStateDef& state) const;
};