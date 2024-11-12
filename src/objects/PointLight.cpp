#include "objects/PointLight.hpp"
#include "DrawState.hpp"
#include <string>

int PointLight::MAX_LIGHT_COUNT = 10;

PointLight::LightNames::LightNames(int id):
	_id(id),
	_colorName(("PointLights[" + std::to_string(id) + "].Color").c_str()),
	_intensityName(("PointLights[" + std::to_string(id) + "].Intensity").c_str()),
	_radiusName(("PointLights[" + std::to_string(id) + "].Radius").c_str()),
	_fadingAreaName(("PointLights[" + std::to_string(id) + "].FadingArea").c_str())
{
}

std::vector<StringId>& PointLight::LightNames::getLightPosNames()
{
	static std::vector<StringId> lightPosNames = initLightPosNames();
	return lightPosNames;
}

std::vector<StringId> PointLight::LightNames::initLightPosNames()
{
	std::vector<StringId> lightPosNames;
	static const std::string nameTemplate = "PointLights";

	for (int i = 0; i < PointLight::MAX_LIGHT_COUNT; i++) {
		std::string name(nameTemplate);
		name += "[" + std::to_string(i) + "]";
		lightPosNames.emplace_back((name + ".Position").c_str());
	}

	return lightPosNames;
}




PointLight::PointLight():
	_position(),
	_color(1.0f, 1.0f, 1.0f),
	_intensity(1.0f),
	_radius(100.0f),
	_fadingArea(0.0f)
{
}

void PointLight::setColor(const glm::vec3& color)
{
	_color = color;
}

void PointLight::setIntensity(float intensity)
{
	_intensity = intensity;
}

void PointLight::setRadius(float radius)
{
	_radius = radius;
}

void PointLight::setFadingArea(float fadingArea)
{
	_fadingArea = fadingArea;
}

void PointLight::setPosition(const glm::vec3& position)
{
	_position = position;
}

void PointLight::predraw(DrawStatePoolDef& statePool)
{
	auto& state = statePool.get();

	static const StringId viewMatrixName = StringId("ViewMatrix");
	const auto& viewMatrix = state.get<glm::mat4>(viewMatrixName);
	const glm::vec4 viewPosition = viewMatrix * glm::vec4(_position, 1.0f);

	const int id = nextFreeId(state);
	if (id < 0) {
		return;
	}

	if (!_names || (_names && _names->getId() != id)) {
		_names = std::make_unique<LightNames>(id);
	}

	state.add(LightNames::getLightPosNames()[id], viewPosition);
	state.add(_names->getColorName(), _color);
	state.add(_names->getIntensityName(), _intensity);
	state.add(_names->getRadiusName(), _radius);
	state.add(_names->getFadingAreaName(), _fadingArea);

	static const StringId pointLightCountName = StringId("PointLightCount");
	state.addOrSet(pointLightCountName, id + 1);
}

int PointLight::nextFreeId(const DrawStateDef& state)
{
	for (int i = 0; i < MAX_LIGHT_COUNT; i++) {

		if (!state.has(LightNames::getLightPosNames()[i])) {
			return i;
		}
	}
	return -1;
}
